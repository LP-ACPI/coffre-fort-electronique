#ifndef COMPRESSCRYPT_H
#define COMPRESSCRYPT_H

#include "Crypt.h"
#include <QFile>
#include <QObject>
#include <QDir>
#include <QDataStream>

class CompressCrypt : public QObject
{
    Q_OBJECT
    explicit CompressCrypt(QObject *parent = 0);
    quint64 totalDataCount;
    bool isBusy = false;

public:
    static CompressCrypt &getInstance(){
        static CompressCrypt instance;
        return instance;
    }

    bool compressDir(const QString&, const QString&);
    bool decompressDir(const QString&,const QString&);

    bool cryptDir(const QString&, const QString&, char*);
    bool decryptDir(const QString&,const QString&, char*);

    bool isCurrentlyBusy(){
        return isBusy;
    }

signals:
    void compressStarted();
    void compressInProgress(quint64,quint64);
    void compressDone();

    void decompressStarted();
    void decompressInProgress(quint64,quint64);
    void decompressDone();

    void cryptingStarted(QString);
    void cryptInProgress(quint64,quint64);
    void cryptingDone();

    void decryptingStarted();
    void decryptInProgress(quint64,quint64);
    void decryptingDone();

    void error(QString);

private:
    QFile file;
    QDataStream dataStream;
    Crypt cryptMan;

    quint64 compress(const QString&,const QString&,quint64);

    quint64 cryptDirWithCount(const QString&,const QString&,quint64,char*);
    quint64 decryptDirWithCount(const QString&,const QString&,quint64,char*);

    quint64 countFiles(const QString&, quint64 initCount = 0);

};


#endif // FOLDERCOMPRESSOR_H
