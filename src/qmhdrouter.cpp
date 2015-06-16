#include "qmhdrouter.h"

#include <QStringList>
#include <QThread>

#include "qmhdcontroller.h"
#include "qmhdrequest.h"
#include "qmhdresponse.h"

static QList<QMHDMethod> methods_from_string(const QString& method)
{
    QList<QMHDMethod> methods;

    foreach (QString string, method.split(',')) {
        QMHDMethod method = qmhd_method_from_string(string.toLocal8Bit().constData());

        if (method != QMHDMethod::Unknown)
            methods.append(method);
    }
    return methods;
}

static QString name_from_slot(const QString& slot)
{
    return slot.left(slot.indexOf('(')).mid(1);
}

class QMHDRouterPrivate
{
    public:
        QThread* findLeastLoadedThread();

    public:
        QList<QMHDRoute> routes;
        QList<QThread*> threads;
};

QThread* QMHDRouterPrivate::findLeastLoadedThread()
{
    QThread* bestOne  = NULL;
    int      bestLoad = 0;

    for (int i = 0; i < threads.count(); ++i) {
        QThread* thread = threads.at(i);
        int      load   = thread->findChildren<QMHDController*>(QString(), Qt::FindDirectChildrenOnly).count();

        if (bestOne == NULL || bestLoad > load)
            bestOne = thread;
    }
    return bestOne;
}

QMHDRouter::QMHDRouter(QObject* parent)
    : QObject(parent),
      d(new QMHDRouterPrivate())
{
    setThreadCount(qMax(4, QThread::idealThreadCount()));
}

QMHDRouter::~QMHDRouter()
{
    setThreadCount(0);
    delete d;
}

void QMHDRouter::processRequest(QMHDRequest* request)
{
    bool found     = false;
    bool processed = false;

    for (int i = 0; !processed && i < d->routes.count(); i++) {
        const QMHDRoute&       route = d->routes.at(i);
        QHash<QString,QString> params;
        bool                   pathOk;
        bool                   methodOk;

        route.match(request->path(), request->method(), &params, &pathOk, &methodOk);
        if (pathOk) {
            if (methodOk) {
                QString method = route.action();

                if (route.receiver() != NULL) {
                    if (params.count() > 0) {
                        QMetaObject::invokeMethod(route.receiver(), qPrintable(method),
                                                  Q_ARG(QMHDRequest*, request),
                                                  Q_ARG(QStringHash, params));
                    } else {
                        QMetaObject::invokeMethod(route.receiver(), qPrintable(method),
                                                  Q_ARG(QMHDRequest*, request));
                    }
                } else if (route.controller() != NULL) {
                    QMHDController* object;
                    QThread*        thread;

                    object = static_cast<QMHDController*>(route.controller()->newInstance());
                    object->setRequest(request);
                    object->setPathParams(params);
                    object->connect(request, &QMHDRequest::destroyed, object, &QObject::deleteLater);

                    thread = d->findLeastLoadedThread();
                    if (thread != NULL)
                        object->moveToThread(thread);

                    QMetaObject::invokeMethod(object, qPrintable(method));
                } else {
                    request->response()->setStatus(QMHDHttpStatus::InternalServerError);
                    request->response()->send();
                }
                processed = true;
            }
            found = true;
        }
    }

    if (found) {
        if (!processed) {
            request->response()->setStatus(QMHDHttpStatus::MethodNotAllowed);
            request->response()->send();
        }
    } else {
        request->response()->setStatus(QMHDHttpStatus::NotFound);
        request->response()->send();
    }
    qDebug("%s %s (found=%d processed=%d)",
           qmhd_method_to_string(request->method()),
           qPrintable(request->path()),
           found,
           processed);
}

void QMHDRouter::addRoute(const QString& method, const QString& path, QObject* receiver, const char* slot)
{
    QMHDRoute route;

    route.setMethods(methods_from_string(method));
    route.setPath(path);
    route.setAction(receiver, name_from_slot(slot));
    d->routes.append(route);
}

void QMHDRouter::addRoute(const QString& method, const QString& path, const QMetaObject* controller, const QString& action)
{
    QMHDRoute route;

    route.setMethods(methods_from_string(method));
    route.setPath(path);
    route.setAction(controller, action);
    d->routes.append(route);
}

const QList<QMHDRoute>& QMHDRouter::routes() const
{
    return d->routes;
}

void QMHDRouter::setRoutes(const QList<QMHDRoute>& routes)
{
    d->routes = routes;
}

int QMHDRouter::threadCount() const
{
    return d->threads.count();
}

void QMHDRouter::setThreadCount(int threadCount)
{
    // add
    while (d->threads.count() < threadCount) {
        QThread* thread;

        thread = new QThread();
        thread->start();
        connect(thread, &QThread::finished,
                thread, &QThread::deleteLater);
        d->threads.append(thread);
    }

    // remove
    while (d->threads.count() > threadCount) {
        d->threads.takeFirst()->quit();
    }
}

