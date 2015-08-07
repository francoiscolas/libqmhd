#ifndef QMHDSERVER_H
#define QMHDSERVER_H

#include <QObject>

class QMHDRequest;
class QMHDServerPrivate;

class QMHDServer : public QObject
{
    Q_OBJECT

    public:
        QMHDServer(QObject* parent = NULL);
        virtual ~QMHDServer();

    public:
        bool isListening() const;
        bool listen(quint16 port = 80);
        void close();

        quint16	port() const;

        int threadPoolSize() const;
        void setThreadPoolSize(int size);

    signals:
        void newRequest(QMHDRequest* request);

    protected:
        QMHDServerPrivate* const d;
};

#endif // QMHDSERVER_H
