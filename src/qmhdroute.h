#ifndef QMHDROUTE_H
#define QMHDROUTE_H

#include "qmhdglobal.h"

class QMetaObject;
class QObject;

class QMHDRoutePrivate;

class QMHDRoute
{
    public:
        QMHDRoute();
        QMHDRoute(const QMHDRoute& route);
        ~QMHDRoute();

    public:
        QMHDRoute& operator=(const QMHDRoute& route);

    public:
        bool match(const QString& path, QMHDMethod method, QHash<QString,QString>* params, bool* pathOk, bool* methodOk) const;

        const QList<QMHDMethod>& methods() const;
        void setMethod(QMHDMethod method);
        void setMethods(const QList<QMHDMethod>& methods);

        const QString& path() const;
        void setPath(const QString& path);

        const QMetaObject* controller() const;
        void setController(const QMetaObject* controller);

        const QString& action() const;
        void setAction(const QString& action);

    public:
        QMHDRoutePrivate* const d;
};

#endif // QMHDROUTE_H
