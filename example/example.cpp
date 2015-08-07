#include <QCoreApplication>
#include <QThread>
#include <QTimer>

#include <qmhdrequest.h>
#include <qmhdresponse.h>
#include <qmhdrouter.h>
#include <qmhdserver.h>

#include "example.h"

// GET /hello
// GET /hello?delay=1000
void Hello::hello()
{
    int delay = request()->query().value("delay").toInt();

    response()->setHeader("Content-Type", "text/plain");
    response()->setHeader("X-Thread-Id", QString::number((quint64) QThread::currentThreadId()));
    if (delay <= 0) {
        response()->send("world\n");
    } else {
        QTimer* timer;

        timer = new QTimer();
        timer->start(delay * 1000);
        connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
        connect(timer, &QTimer::timeout, [=]() {
            response()->send("world (delayed)\n");
        });
    }
}

void startSimple()
{
    QMHDServer* server = new QMHDServer(qApp);

    server->connect(server, &QMHDServer::newRequest, server, [=](QMHDRequest* req) {
        if (req->query().contains("delay"))
            QThread::sleep(req->query().value("delay").toInt());
        req->response()->setHeader("Content-Type", "text/plain");
        req->response()->setHeader("X-Thread-Id", QString::number((quint64) QThread::currentThreadId()));
        req->response()->send("Yo\n");
    }, Qt::DirectConnection);
    if (!server->listen(8080))
        exit(1);
}

void startRouter()
{
    QMHDServer* server = new QMHDServer(qApp);
    QMHDRouter* router = new QMHDRouter(qApp);

    router->addRoute("GET", "/hello", &Hello::staticMetaObject, "hello");
    router->connect(server, &QMHDServer::newRequest,
                    router, &QMHDRouter::processRequest,
                    Qt::DirectConnection);
    if (!server->listen(8181))
        exit(2);
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    startSimple();
    startRouter();
    return app.exec();
}
