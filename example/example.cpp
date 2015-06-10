#include <QCoreApplication>
#include <QTimer>

#include <qmhdrequest.h>
#include <qmhdresponse.h>
#include <qmhdrouter.h>
#include <qmhdserver.h>

#include "example.h"

static void do_hello(QMHDResponse* response)
{
    response->setHeader("Content-Type", "text/plain");
    response->send("world\n");
}

static void do_world(QMHDResponse* response)
{
    response->setHeader("Content-Type", "text/plain");
    response->send("hello\n");
}

// GET /hello
// GET /hello?delay=1000
void Hello::hello(QMHDRequest* request)
{
    int delay = request->query().value("delay").toInt();

    if (delay <= 0) {
        do_hello(request->response());
    } else {
        QTimer* timer;

        timer = new QTimer();
        timer->start(delay);
        connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
        connect(timer, &QTimer::timeout, [=]() {
            do_hello(request->response());
        });
    }
}

// GET /world
// GET /world?delay=1000
void World::world()
{
    int delay = params().value("delay").toInt();

    if (delay <= 0) {
        do_world(response());
    } else {
        QTimer* timer;

        timer = new QTimer();
        timer->start(delay);
        connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
        connect(timer, &QTimer::timeout, [=]() {
            do_world(response());
        });
    }
}

void startSimple()
{
    QMHDServer* server = new QMHDServer(qApp);

    server->connect(server, &QMHDServer::newRequest, server, [=](QMHDRequest* req) {
        req->response()->setHeader("Content-Type", "text/plain");
        req->response()->send("Yo\n");
    });
    if (!server->listen(8080))
        exit(1);
}

void startRouter()
{
    QMHDServer* server = new QMHDServer(qApp);
    QMHDRouter* router = new QMHDRouter(qApp);

    router->addRoute("GET", "/hello", new Hello(), SLOT(hello(QMHDRequest*)));
    router->addRoute("GET", "/world", &World::staticMetaObject, "world");
    router->connect(server, &QMHDServer::newRequest, router, &QMHDRouter::processRequest);
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
