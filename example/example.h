#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <qmhdcontroller.h>

class Hello : public QMHDController
{
    Q_OBJECT
    QMHDCONTROLLER(Hello)

    public slots:
        void hello();
};

#endif // EXAMPLE_H

