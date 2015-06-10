#include "qmhdcontroller.h"

#include <QUrlQuery>

#include "qmhdbody.h"
#include "qmhdrequest.h"

class QMHDControllerPrivate
{
    public:
        QMHDControllerPrivate();

    public:
        QMHDRequest* request;
        QStringHash pathParams;
};

QMHDControllerPrivate::QMHDControllerPrivate()
    : request(NULL)
{
}

QMHDController::QMHDController(QObject* parent)
    : QObject(parent),
      d(new QMHDControllerPrivate())
{
}

QMHDController::~QMHDController()
{
    delete d;
}

QMHDRequest* QMHDController::request() const
{
    return d->request;
}

QMHDResponse* QMHDController::response() const
{
    return d->request->response();
}

QStringHash QMHDController::params() const
{
    return QStringHash()
            .unite(pathParams())
            .unite(queryParams())
            .unite(bodyParams());
}

const QStringHash& QMHDController::pathParams() const
{
    return d->pathParams;
}

const QStringHash& QMHDController::queryParams() const
{
    return d->request->query();
}

const QStringHash& QMHDController::bodyParams() const
{
    return d->request->body().params();
}

void QMHDController::setRequest(QMHDRequest* request)
{
    d->request = request;
}

void QMHDController::setPathParams(const QStringHash& pathParams)
{
    d->pathParams = pathParams;
}
