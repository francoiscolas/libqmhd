#include "qmhdbody.h"

#include <QTemporaryFile>

#include "qmhdbodyfile.h"

class QMHDBodyPrivate
{
    public:
        QStringHash params;
        QMHDBodyFiles files;
};

QMHDBody::QMHDBody()
    : d(new QMHDBodyPrivate())
{
}

QMHDBody::~QMHDBody()
{
    for (auto it = d->files.begin(); it != d->files.end(); ++it)
        delete it.value();
    delete d;
}

QString QMHDBody::param(const QString& name) const
{
    return d->params.value(name);
}

bool QMHDBody::paramExists(const QString& name) const
{
    return d->params.contains(name);
}

const QStringHash& QMHDBody::params() const
{
    return d->params;
}

QMHDBodyFile* QMHDBody::file(const QString& name) const
{
    return d->files.value(name);
}

bool QMHDBody::fileExists(const QString& name) const
{
    return d->files.contains(name);
}

const QMHDBodyFiles& QMHDBody::files() const
{
    return d->files;
}

void QMHDBody::setParam(const QString& name, const QString& value)
{
    d->params.insertMulti(name, value);
}

void QMHDBody::setFile(const QString& name, QMHDBodyFile* file)
{
    if (d->files.contains(name))
        delete d->files.take(name);
    d->files.insert(name, file);
}
