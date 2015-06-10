#ifndef QMHDBODYFILE_H
#define QMHDBODYFILE_H

#include <QString>

class QTemporaryFile;

class QMHDBodyFilePrivate;

class QMHDBodyFile
{
    public:
        QMHDBodyFile(const QString& filename, const QString& contentType);
        virtual ~QMHDBodyFile();

    private:
        QMHDBodyFile(const QMHDBodyFile& other);
        QMHDBodyFile& operator=(const QMHDBodyFile& other);

    public:
        const QString& filename() const;
        const QString& contentType() const;
        QTemporaryFile* file() const;

    public:
        QMHDBodyFilePrivate* const d;
};

#endif // QMHDBODYFILE_H
