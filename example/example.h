#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <qmhdcontroller.h>

class Hello : public QMHDController
{
    Q_OBJECT

    public:
        Q_INVOKABLE
        Hello(QObject* parent = NULL);

    public slots:
        void hello();
};

#endif // EXAMPLE_H

