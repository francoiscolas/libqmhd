#include "qmhdbodyfile.h"

#include <QTemporaryFile>

class QMHDBodyFilePrivate
{
    public:
        QString filename;
        QString contentType;
        QTemporaryFile* file;
};

QMHDBodyFile::QMHDBodyFile(const QString& filename, const QString& contentType)
    : d(new QMHDBodyFilePrivate())
{
    d->filename = filename;
    d->contentType = contentType;
    d->file = new QTemporaryFile();
    d->file->open();
}

QMHDBodyFile::~QMHDBodyFile()
{
    delete d->file;
    delete d;
}

const QString& QMHDBodyFile::filename() const
{
    return d->filename;
}

const QString& QMHDBodyFile::contentType() const
{
    return d->contentType;
}

QTemporaryFile* QMHDBodyFile::file() const
{
    return d->file;
}
