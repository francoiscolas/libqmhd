#include "qmhdrouter.h"

#include <QEventLoop>
#include <QReadWriteLock>
#include <QStringList>
#include <QTimer>

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

class QMHDRouterPrivate
{
    public:
        void processAction(const QMHDRoute& route, QMHDRequest* request, const QHash<QString,QString>& params);

    public:
        QReadWriteLock lock;
        QList<QMHDRoute> routes;
};

void QMHDRouterPrivate::processAction(const QMHDRoute& route, QMHDRequest* request, const QHash<QString, QString>& params)
{
    QEventLoop eventLoop;
    QTimer     timer;
    QString    action = route.action();
    QString    method = route.action() + "()";

    if (route.controller() != NULL && route.controller()->indexOfMethod(qPrintable(method)) >= 0) {
        QMHDController* object;

        object = static_cast<QMHDController*>(route.controller()->newInstance());
        object->setParent(request);
        object->setRequest(request);
        object->setPathParams(params);
        QObject::connect(request->response(), &QMHDResponse::sent, &eventLoop, &QEventLoop::quit);
        QObject::connect(&timer, &QTimer::timeout, [=]() {
            QMetaObject::invokeMethod(object, qPrintable(action));
        });
        timer.setSingleShot(true);
        timer.start();
        eventLoop.exec();
    } else {
        request->response()->setStatus(QMHDHttpStatus::InternalServerError);
        request->response()->send();
    }
}

QMHDRouter::QMHDRouter(QObject* parent)
    : QObject(parent),
      d(new QMHDRouterPrivate())
{
}

QMHDRouter::~QMHDRouter()
{
    delete d;
}

void QMHDRouter::processRequest(QMHDRequest* request)
{
    QReadLocker locker(&d->lock);
    bool        found     = false;
    bool        processed = false;

    for (int i = 0; !processed && i < d->routes.count(); i++) {
        const QMHDRoute&       route = d->routes.at(i);
        QHash<QString,QString> params;
        bool                   pathOk;
        bool                   methodOk;

        route.match(request->path(), request->method(), &params, &pathOk, &methodOk);
        if (pathOk) {
            if (methodOk) {
                d->processAction(route, request, params);
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

void QMHDRouter::addRoute(const QString& method, const QString& path,
                          const QMetaObject* controller, const QString& action)
{
    QWriteLocker locker(&d->lock);
    QMHDRoute    route;

    route.setMethods(methods_from_string(method));
    route.setPath(path);
    route.setController(controller);
    route.setAction(action);
    d->routes.append(route);
}

const QList<QMHDRoute>& QMHDRouter::routes() const
{
    return d->routes;
}

void QMHDRouter::setRoutes(const QList<QMHDRoute>& routes)
{
    d->lock.lockForWrite();
    d->routes = routes;
    d->lock.unlock();
}
