#ifndef QMHDREQUEST_H
#define QMHDREQUEST_H

#include <QObject>

#include "qmhdglobal.h"

class QMHDBody;
class QMHDRequestPrivate;
class QMHDResponse;

class QMHDRequest : public QObject
{
    Q_OBJECT

    friend class QMHDServerPrivate;

    private:
        QMHDRequest(void* mhdConnection);

    public:
        virtual ~QMHDRequest();

    public:
        QMHDMethod method() const;
        QString methodString() const;
        const QString& path() const;
        const QStringHash& query() const;
        QMHDHttpVersion httpVersion() const;
        QString header(const QString& name) const;
        const QStringHash& headers() const;
        const QMHDBody& body() const;
        QMHDResponse* response() const;

    private:
        quint64 parseBody(const char* data, quint64 length);
        void setMethod(QMHDMethod method);
        void setPath(const QString& path);
        void setQuery(const QStringHash& query);
        void setHttpVersion(QMHDHttpVersion httpVersion);
        void setHeader(const QString& name, const QString& value);
        void setHeaders(const QStringHash& headers);
        void setResponse(QMHDResponse* response);

    private:
        QMHDRequestPrivate* const d;
};

#endif // QMHDREQUEST_H
