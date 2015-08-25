#ifndef QMHDCONTROLLER_H
#define QMHDCONTROLLER_H

#include <QHash>
#include <QObject>

#include "qmhdglobal.h"

class QMHDRequest;
class QMHDResponse;

class QMHDControllerPrivate;

class QMHDController : public QObject
{
    Q_OBJECT

    friend class QMHDRouterPrivate;

    protected:
        QMHDController(QObject* parent = NULL);

    public:
        virtual ~QMHDController();

    public:
        virtual QMHDController* clone(QObject* parent = NULL) const;

    public:
        QMHDRequest* request() const;
        QMHDResponse* response() const;

        QString param(const QString& name) const;
        QStringHash params() const;
        const QStringHash& pathParams() const;
        const QStringHash& queryParams() const;
        const QStringHash& bodyParams() const;

    private:
        void setRequest(QMHDRequest* request);
        void setPathParams(const QStringHash& pathParams);

    private:
        QMHDControllerPrivate* const d;
};

#endif // QMHDCONTROLLER_H
