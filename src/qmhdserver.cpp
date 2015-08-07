#include "qmhdserver.h"

#include <QThread>
#include <QUrl>

#include <microhttpd.h>

#include "qmhdrequest.h"
#include "qmhdresponse.h"

class QMHDServerPrivate
{
    public:
        QMHDServerPrivate(QMHDServer* q);

    public:
        int onRequest(MHD_Connection* mhdConnection,
                      const char* method,
                      const char* path,
                      const char* httpVersion,
                      void** requestPtr);
        int onRequestBody(QMHDRequest* request, const char* body, size_t* length);
        int onRequestDone(QMHDRequest* request);

    public:
        QMHDServer* const q;
        MHD_Daemon* mhdDaemon;
        int threadPoolSize;
};

static int on_request_cb(void* dServerPtr,
                         MHD_Connection* mhdConnection,
                         const char* url,
                         const char* method,
                         const char* httpVersion,
                         const char* body,
                         size_t* bodySize,
                         void** requestPtr)
{
    QMHDServerPrivate* server  = static_cast<QMHDServerPrivate*>(dServerPtr);
    QMHDRequest*       request = static_cast<QMHDRequest*>(*requestPtr);

    if (request == NULL) {
        return server->onRequest(mhdConnection, method, url, httpVersion, requestPtr);
    } else if (*bodySize > 0) {
        return server->onRequestBody(request, body, bodySize);
    } else {
        return server->onRequestDone(request);
    }
}

static void on_response_sent_cb(void* /*dServerPtr*/,
                                MHD_Connection* /*mhdConnection*/,
                                void** requestPtr,
                                MHD_RequestTerminationCode /*rtc*/)
{
    QMHDRequest* request = static_cast<QMHDRequest*>(*requestPtr);

    request->response()->deleteLater();
    request->deleteLater();
}

static int header_values_iterator(void* qstringHashPtr,
                                  MHD_ValueKind /*valueKind*/,
                                  const char* key,
                                  const char* value)
{
    static_cast<QStringHash*>(qstringHashPtr)->insertMulti(key, value);
    return MHD_YES;
}

QMHDServerPrivate::QMHDServerPrivate(QMHDServer* q)
    : q(q),
      mhdDaemon(NULL),
      threadPoolSize(QThread::idealThreadCount())
{
}

int QMHDServerPrivate::onRequest(MHD_Connection* mhdConnection,
                                 const char* method,
                                 const char* path,
                                 const char* httpVersion,
                                 void** requestPtr)
{
    QMHDRequest* request = new QMHDRequest(mhdConnection);
    QStringHash  query;
    QStringHash  headers;

    MHD_get_connection_values(mhdConnection, MHD_GET_ARGUMENT_KIND, &header_values_iterator, &query);
    MHD_get_connection_values(mhdConnection, MHD_HEADER_KIND, &header_values_iterator, &headers);
    request->setMethod(qmhd_method_from_string(method));
    request->setPath(path);
    request->setQuery(query);
    request->setHttpVersion(qmhd_http_version_from_string(httpVersion));
    request->setHeaders(headers);
    request->setResponse(new QMHDResponse(mhdConnection));
    *requestPtr = request;
    return MHD_YES;
}

int QMHDServerPrivate::onRequestBody(QMHDRequest* request, const char* body, size_t* length)
{
    *length -= request->parseBody(body, *length);
    return (*length == 0) ? MHD_YES : MHD_NO;
}

int QMHDServerPrivate::onRequestDone(QMHDRequest* request)
{
    emit q->newRequest(request);
    return MHD_YES;
}

QMHDServer::QMHDServer(QObject* parent)
    : QObject(parent),
      d(new QMHDServerPrivate(this))
{
}

QMHDServer::~QMHDServer()
{
    delete d;
}

bool QMHDServer::isListening() const
{
    return (d->mhdDaemon != NULL);
}

bool QMHDServer::listen(quint16 port)
{
    if (d->mhdDaemon == NULL) {
        d->mhdDaemon = MHD_start_daemon(
                           MHD_USE_DEBUG
                           | MHD_USE_POLL_INTERNALLY
                           | MHD_USE_THREAD_PER_CONNECTION
                           | MHD_USE_PEDANTIC_CHECKS,
                           port,
                           NULL, NULL,
                           &on_request_cb, d,
                           MHD_OPTION_NOTIFY_COMPLETED, &on_response_sent_cb, d,
                           MHD_OPTION_THREAD_POOL_SIZE, d->threadPoolSize,
                           MHD_OPTION_END);
    }
    return isListening();
}

void QMHDServer::close()
{
    if (d->mhdDaemon != NULL) {
        MHD_stop_daemon(d->mhdDaemon);
        d->mhdDaemon = NULL;
    }
}

int QMHDServer::threadPoolSize() const
{
    return d->threadPoolSize;
}

void QMHDServer::setThreadPoolSize(int size)
{
    if (!isListening())
        d->threadPoolSize = size;
}
