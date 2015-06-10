#ifndef QMHDBODY_H
#define QMHDBODY_H

#include <QHash>
#include <QString>

#include "qmhdglobal.h"

class QMHDBodyFile;
class QMHDBodyPrivate;

typedef QHash<QString,QMHDBodyFile*> QMHDBodyFiles;

class QMHDBody
{
    friend class QMHDRequestPrivate;

    public:
        QMHDBody();
        ~QMHDBody();

    private:
        QMHDBody(const QMHDBody& other);
        QMHDBody& operator=(const QMHDBody& other);

    public:
        QString param(const QString& name) const;
        bool paramExists(const QString& name) const;
        const QStringHash& params() const;

        QMHDBodyFile* file(const QString& name) const;
        bool fileExists(const QString& name) const;
        const QMHDBodyFiles& files() const;

    private:
        void setParam(const QString& name, const QString& value);
        void setFile(const QString& name, QMHDBodyFile* file);

    protected:
        QMHDBodyPrivate* const d;
};

#endif // QMHDBODY_H
