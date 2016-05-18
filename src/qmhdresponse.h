#ifndef QMHDRESPONSE_H
#define QMHDRESPONSE_H

#include <QObject>

#include "qmhdglobal.h"

class QFileDevice;
class QIODevice;
class QJsonDocument;

class QMHDResponsePrivate;

#define DEFAULT_DEVICE_BLOCK_SIZE 1024

class QMHDResponse : public QObject
{
    Q_OBJECT

    friend class QMHDServerPrivate;

    private:
        QMHDResponse(void* mhdConnection);

    public:
        virtual ~QMHDResponse();

    public:
        void send(const QByteArray& buffer);
        void send(const QJsonDocument& json);
        void send(QIODevice* device,
                  size_t blockSize=DEFAULT_DEVICE_BLOCK_SIZE);
        void send(int fileDescriptor, size_t fileSize);
        void send(int fileDescriptor, off_t fileOffset, size_t fileSize);
        void send();

        QMHD::HttpStatus status() const;
        void setStatus(QMHD::HttpStatus status);

        QString header(const QString& name) const;
        void setHeader(const QString& name, const QString& value);

        QStringHash headers() const;
        void setHeaders(const QStringHash& headers);

    signals:
        void sent();

    private:
        QMHDResponsePrivate* const d;
};

#endif // QMHDRESPONSE_H
