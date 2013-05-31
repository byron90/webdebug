#ifndef WDDEF_H
#define WDDEF_H

#include <QMutexLocker>
#include <QStringList>

//summary of a request
typedef struct {
    char caMethod[15] ;	//request method
    char * pcHost ;			//host
    char * pcUrl ;			//url
}REQUSUM, *PREQUSUM ;

//summary of a response
typedef struct {
    char caCode[4] ;		//response code
    char caStat[20] ;	//statement of code
    int iLenCon ;		//content-length
    char * pcType ;			//content type
}RESPSUM, *PRESPSUM ;

//one request and response info
typedef struct {
    char * pcRequRaw ;
    char * pcRespRaw ;
    int iLenRequ ;	//request packet length
    int iLenResp ;	//response packet length
    PREQUSUM pRequSum ;	//summary of a request package
    PRESPSUM pRespSum ;	//summary of a response package
}DATANODE, *PDATANODE ;

//block url list management
typedef struct {
    QStringList qslstUrl;
    QMutex qmtxLock;
    QString qstrFilePath;
}BLOCKURL, *PBLOCKURL;

#endif // WDDEF_H
