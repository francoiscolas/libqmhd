#ifndef QMHDROUTER_H
#define QMHDROUTER_H

#include <QObject>

#include "qmhdroute.h"

class QMHDRequest;
class QMHDRouterPrivate;

class QMHDRouter : public QObject
{
    Q_OBJECT

    public:
        QMHDRouter(QObject* parent = NULL);
        virtual ~QMHDRouter();

    public:
        void processRequest(QMHDRequest* request);

        void addRoute(const QString& method, const QString& path,
                      const QMetaObject* controller, const QString& action);

        const QList<QMHDRoute>& routes() const;
        void setRoutes(const QList<QMHDRoute>& routes);

    public:
        QMHDRouterPrivate* const d;
};

#endif // QMHDROUTER_H
