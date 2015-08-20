#ifndef QMHDROUTE_H
#define QMHDROUTE_H

#include "qmhdglobal.h"

class QObject;

class QMHDController;
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
        bool match(const QString& path, QMHD::Method verb, QHash<QString,QString>* params, bool* pathOk, bool* methodOk) const;

        const QList<QMHD::Method>& httpVerbs() const;
        void setHttpVerbs(QMHD::Method verb);
        void setHttpVerbs(const QList<QMHD::Method>& verbs);

        const QString& path() const;
        void setPath(const QString& path);

        QMHDController* controller() const;
        void setController(QMHDController* controller);

        const QString& action() const;
        void setAction(const QString& action);

    public:
        QMHDRoutePrivate* const d;
};

#endif // QMHDROUTE_H
