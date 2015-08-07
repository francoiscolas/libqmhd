#include "qmhdroute.h"

#include <QList>
#include <QRegularExpression>

class QMHDRoutePrivate
{
    public:
        QMHDRoutePrivate();
        QMHDRoutePrivate(const QMHDRoutePrivate& other);

    public:
        QMHDRoutePrivate& operator=(const QMHDRoutePrivate& other);

    public:
        QList<QMHDMethod> methods;
        QString path;
        QStringList pathKeys;
        QRegularExpression pathRe;
        const QMetaObject* controller;
        QString action;
};

QMHDRoutePrivate::QMHDRoutePrivate()
    : controller(NULL)
{
}

QMHDRoutePrivate::QMHDRoutePrivate(const QMHDRoutePrivate& other)
{
    *this = other;
}

QMHDRoutePrivate& QMHDRoutePrivate::operator=(const QMHDRoutePrivate& other)
{
    methods = other.methods;
    path = other.path;
    pathKeys = other.pathKeys;
    pathRe = other.pathRe;
    controller = other.controller;
    action = other.action;
    return *this;
}

QMHDRoute::QMHDRoute()
    : d(new QMHDRoutePrivate())
{
}

QMHDRoute::QMHDRoute(const QMHDRoute& route)
    : d(new QMHDRoutePrivate(*(route.d)))
{
}

QMHDRoute::~QMHDRoute()
{
    delete d;
}

QMHDRoute& QMHDRoute::operator=(const QMHDRoute& route)
{
    *d = *route.d;
    return *this;
}

bool QMHDRoute::match(const QString& path, QMHDMethod method, QHash<QString,QString>* paramsPtr, bool* pathOkPtr, bool* methodOkPtr) const
{
    QRegularExpressionMatchIterator matchIt;
    QHash<QString,QString>          params;
    bool                            pathOk;
    bool                            methodOk;

    pathOk = false;
    if ((matchIt = d->pathRe.globalMatch(path)).hasNext()) {
        pathOk = true;
        if (d->pathKeys.count() > 0) {
            for (int i = 0; matchIt.hasNext(); ++i) {
                QRegularExpressionMatch match = matchIt.next();
                params.insert(d->pathKeys.at(i), match.captured(1));
            }
        }
    }

    methodOk = false;
    if (d->methods.contains(method)
            || (method == QMHDMethod::HEAD && d->methods.contains(QMHDMethod::GET))) {
        methodOk = true;
    }

    if (paramsPtr != NULL)
        *paramsPtr = params;

    if (pathOkPtr != NULL)
        *pathOkPtr = pathOk;

    if (methodOkPtr != NULL)
        *methodOkPtr = methodOk;

    return (pathOk && methodOk);
}

const QList<QMHDMethod>& QMHDRoute::methods() const
{
    return d->methods;
}

void QMHDRoute::setMethod(QMHDMethod method)
{
    d->methods.clear();
    d->methods.append(method);
}

void QMHDRoute::setMethods(const QList<QMHDMethod>& methods)
{
    d->methods = methods;
}

const QString& QMHDRoute::path() const
{
    return d->path;
}

void QMHDRoute::setPath(const QString& path)
{
    QString                         pattern("^" + path + "$");
    QRegularExpressionMatchIterator matchIt;

    d->path = path;
    d->pathKeys.clear();
    if ((matchIt = QRegularExpression(":(\\w+)(\\(.*\\))?").globalMatch(path)).hasNext()) {
        while (matchIt.hasNext()) {
            QRegularExpressionMatch match = matchIt.next();
            QString                 c     = match.captured(0);
            QString                 k     = match.captured(1);
            QString                 p     = match.captured(2);

            if (p.isEmpty())
                p = "([a-zA-Z0-9-$_.+!*'(),%]+)"; // According to RFC-1738 (http://www.rfc-editor.org/rfc/rfc1738.txt).
            pattern.replace(c, p);
            d->pathKeys.append(k);
        }
        d->pathRe.setPattern(pattern);
    } else {
        d->pathRe.setPattern(path);
    }
}

const QMetaObject* QMHDRoute::controller() const
{
    return d->controller;
}

void QMHDRoute::setController(const QMetaObject* controller)
{
    d->controller = controller;
}

const QString& QMHDRoute::action() const
{
    return d->action;
}

void QMHDRoute::setAction(const QString& action)
{
    d->action = action;
}
