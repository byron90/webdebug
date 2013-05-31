#include <QFile>
#include "wdglobal.h"

WDGlobal::WDGlobal(QObject *parent) :
    QObject(parent)
{
    mpBlockUrl = new BLOCKURL;
}

//init block url struct
//read urls from file qstrFileName
void WDGlobal::initBlockUrl(QString &qstrFile)
{
    mpBlockUrl->qstrFilePath = qstrFile;
    if(qstrFile!="")
    {
        QFile qdocUrls(qstrFile);
        if(!qdocUrls.open(QIODevice::ReadOnly))
            return;
        QString qstrRawUrls(qdocUrls.readAll());
        QStringList qslstTemp = qstrRawUrls.split('\n');
        for(int i=0; i<qslstTemp.size(); i++)
            mpBlockUrl->qslstUrl<<qslstTemp[i];
        qdocUrls.close();
    }
}
