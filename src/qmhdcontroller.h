#ifndef QMHDCONTROLLER_H
#define QMHDCONTROLLER_H

#include <QHash>
#include <QObject>

#include "qmhdglobal.h"

class QMHDRequest;
class QMHDResponse;

class QMHDControllerPrivate;

#define QMHDCONTROLLER(Type)            \
    public:                             \
        Q_INVOKABLE                     \
        Type(QObject* parent = NULL)    \
            : QMHDController(parent) {} \

class QMHDController : public QObject
{
    Q_OBJECT

    friend class QMHDRouter;

    protected:
        QMHDController(QObject* parent = NULL);
        virtual ~QMHDController();

    public:
        QMHDRequest* request() const;
        QMHDResponse* response() const;

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
