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
        QList<QMHD::Method> httpVerbs;
        QString path;
        QStringList pathKeys;
        QRegularExpression pathRe;
        QMHDController* controller;
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
    httpVerbs = other.httpVerbs;
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

bool QMHDRoute::match(const QString& path, QMHD::Method verb, QHash<QString,QString>* paramsPtr, bool* pathOkPtr, bool* methodOkPtr) const
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
    if (d->httpVerbs.contains(verb)
            || (verb == QMHD::HEAD && d->httpVerbs.contains(QMHD::GET))) {
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

const QList<QMHD::Method>& QMHDRoute::httpVerbs() const
{
    return d->httpVerbs;
}

void QMHDRoute::setHttpVerbs(QMHD::Method verb)
{
    d->httpVerbs.clear();
    d->httpVerbs.append(verb);
}

void QMHDRoute::setHttpVerbs(const QList<QMHD::Method>& verbs)
{
    d->httpVerbs = verbs;
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

QMHDController* QMHDRoute::controller() const
{
    return d->controller;
}

void QMHDRoute::setController(QMHDController* controller)
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
