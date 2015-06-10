#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <qmhdcontroller.h>

class Hello : public QObject
{
    Q_OBJECT

    public slots:
        void hello(QMHDRequest* request);
};

class World : public QMHDController
{
    Q_OBJECT
    QMHDCONTROLLER(World)

    public slots:
        void world();
};

#endif // EXAMPLE_H

