Libqmhd
=======

Libqmhd is a Qt wrapper for [libmicrohttpd](http://www.gnu.org/software/libmicrohttpd/). It handle requests in a background thread then you can handle the request by connecting to the `QMHDServer::newRequest()` signal. Libqmhd also comes with a router to easily route requests to your code. You can use it in two ways. One is by connecting a route to a QObject's slot in which case the request will be executed in the receiver's thread. The other way is by connecting a route to a custom `QMHDController` object in which case the request will be executed in one of the threads of the router's thread pool.

Usage
-----

The simplest usage of libqmhd is to create a `QMHDServer` then connect to the `newRequest()` signal:
```c++
QMHDServer* server = new QMHDServer();

connect(server, &QMHDServer::newRequest, server, [=](QMHDRequest* request) {
    request->response()->setHeader("Content-Type", "text/plain");
    request->response()->send("Yo\n");
});
if (!server->listen(8080)) {
    // handle error
}
```

You can also use a `QMHDRouter` to handle requests more easily and route them to your code. In the example below `hello` is a QObject and the request is executed in `hello`'s thread:
```c++
QMHDServer* server = new QMHDServer();
QMHDRouter* router = new QMHDRouter();

router->addRoute("GET", "/hello", hello, SLOT(handleRequest(QMHDRequest*)));
connect(server, &QMHDServer::newRequest, router, &QMHDRouter::processRequest);
if (!server->listen(8080)) {
    // handle error
}
```

The last example uses a `QMHDController`. In this case the request will be executed in one of the threads of the router's thread pool:
```c++
class Hello : public QMHDController
{
    Q_OBJECT
    QMHDCONTROLLER(Hello)

    public slots:
        void hello()
        {
            response()->setHeader("Content-Type", "text/plain");
            response()->send("world\n");
        }
};

QMHDServer* server = new QMHDServer();
QMHDRouter* router = new QMHDRouter();

router->addRoute("GET", "/hello", &Hello::staticMetaObject, "hello");
connect(server, &QMHDServer::newRequest, router, &QMHDRouter::processRequest);
if (!server->listen(8080)) {
    // handle error
}
```
