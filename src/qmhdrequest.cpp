#include "qmhdrequest.h"

#include <QTemporaryFile>

#include <microhttpd.h>

#include "qmhdbody.h"
#include "qmhdbodyfile.h"
#include "qmhdresponse.h"

class QMHDRequestPrivate
{
    public:
        static int bodyIterator(void* dRequestPtr,
                                MHD_ValueKind valueKind,
                                const char* key,
                                const char* filename,
                                const char* contentType,
                                const char* transfer_encoding,
                                const char* data,
                                uint64_t dataOffset,
                                size_t dataSize);

    public:
        QMHDRequestPrivate(MHD_Connection* mhdConnection);
        ~QMHDRequestPrivate();

    public:
        quint64 parseBody(const char* data, quint64 length);

    public:
        MHD_Connection* const mhdConnection;
        MHD_PostProcessor* mhdBodyProcessor;
        QMHDResponse* response;
        QMHDMethod method;
        QString path;
        QStringHash query;
        QMHDHttpVersion httpVersion;
        QStringHash headers;
        QMHDBody body;
};

int QMHDRequestPrivate::bodyIterator(void* dRequestPtr,
                                     MHD_ValueKind /*valueKind*/,
                                     const char* key,
                                     const char* filename,
                                     const char* contentType,
                                     const char* /*transfer_encoding*/,
                                     const char* data,
                                     uint64_t dataOffset,
                                     size_t dataSize)
{
    QMHDRequestPrivate* request = static_cast<QMHDRequestPrivate*>(dRequestPtr);
    QMHDBody*           body    = &(request->body);

    if (filename == NULL) {
        body->setParam(key, QByteArray::fromRawData(data + dataOffset, dataSize));
    } else {
        QMHDBodyFile* upload = body->file(filename);
        int           n;

        if (upload == NULL) {
            upload = new QMHDBodyFile(filename, contentType);
            body->setFile(filename, upload);
        }
        while (dataSize > 0) {
            n = upload->file()->write(data + dataOffset, dataSize);
            if (n < 0)
                return MHD_NO;
            if (!upload->file()->flush())
                return MHD_NO;
            dataOffset += n;
            dataSize   -= n;
        }
    }
    return MHD_YES;
}

QMHDRequestPrivate::QMHDRequestPrivate(MHD_Connection* mhdConnection)
    : mhdConnection(mhdConnection),
      mhdBodyProcessor(NULL),
      response(NULL),
      method(QMHDMethod::GET),
      httpVersion(QMHDHttpVersion::Unknown)
{
}

QMHDRequestPrivate::~QMHDRequestPrivate()
{
    MHD_destroy_post_processor(mhdBodyProcessor);
}

quint64 QMHDRequestPrivate::parseBody(const char* data, quint64 length)
{
    if (mhdBodyProcessor == NULL) {
        mhdBodyProcessor = MHD_create_post_processor(mhdConnection, 1024, &QMHDRequestPrivate::bodyIterator, this);
        if (mhdBodyProcessor == NULL)
            return -1;
    }
    if (MHD_post_process(mhdBodyProcessor, data, length) == MHD_NO) {
        return -1;
    }
    return length;
}

QMHDRequest::QMHDRequest(void* mhdConnection)
    : QObject(),
      d(new QMHDRequestPrivate(static_cast<MHD_Connection*>(mhdConnection)))
{
}

QMHDRequest::~QMHDRequest()
{
    delete d;
}

QMHDMethod QMHDRequest::method() const
{
    return d->method;
}

const QString& QMHDRequest::path() const
{
    return d->path;
}

const QStringHash& QMHDRequest::query() const
{
    return d->query;
}

QMHDHttpVersion QMHDRequest::httpVersion() const
{
    return d->httpVersion;
}

QString QMHDRequest::header(const QString& name) const
{
    return d->headers.value(name.toLower());
}

const QStringHash& QMHDRequest::headers() const
{
    return d->headers;
}

const QMHDBody& QMHDRequest::body() const
{
    return d->body;
}

QMHDResponse* QMHDRequest::response() const
{
    return d->response;
}

quint64 QMHDRequest::parseBody(const char* data, quint64 length)
{
    return d->parseBody(data, length);
}

void QMHDRequest::setMethod(QMHDMethod method)
{
    d->method = method;
}

void QMHDRequest::setPath(const QString& path)
{
    d->path = path;
}

void QMHDRequest::setQuery(const QStringHash& query)
{
    d->query = query;
}

void QMHDRequest::setHttpVersion(QMHDHttpVersion httpVersion)
{
    d->httpVersion = httpVersion;
}

void QMHDRequest::setHeader(const QString& name, const QString& value)
{
    d->headers.insert(name.toLower(), value);
}

void QMHDRequest::setHeaders(const QStringHash& headers)
{
    d->headers.clear();
    for (auto it = headers.begin(); it != headers.end(); ++it)
        setHeader(it.key(), it.value());
}

void QMHDRequest::setResponse(QMHDResponse* response)
{
    d->response = response;
}

