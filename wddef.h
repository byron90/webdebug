#ifndef WDDEF_H
#define WDDEF_H

#include <QMutexLocker>
#include <QWaitCondition>
#include <QStringList>
#include <QFile>

//logger
typedef struct{
    QFile *pqdocLog;
    QMutex qmtxLock;
}LOGGER, *PLOGGER;

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

//proxy
typedef struct{
    QString qstrProxyHost;
    QString qstrProxyPort;
    QMutex qmtxLock;
}PROXYCONF, *PPROXYCONF;

//block url list manager
typedef struct {
    QStringList qslstUrl;
    QMutex qmtxLock;
    QString qstrFilePath;
}BLOCKURL, *PBLOCKURL;

//auto response node
typedef struct st{
    QString qstrUrl;
    QString qstrSrcPath;
    //overload ==
    friend bool operator ==(st &a, const st &b)
    {
        return a.qstrUrl==b.qstrUrl && a.qstrSrcPath==b.qstrSrcPath;
    }
}AUTORNODE, *PAUTORNODE;

//auto response manager
typedef struct{
    QList<AUTORNODE> qlstNodes;
    QString qstrFilePath;
    QMutex  qmtxLock;
}AUTOR, *PAUTOR;

//hide response with some code
typedef struct{
    bool b2;	//is 2** hided;
    bool b302;
    bool b304;
    bool bnot2;
    QMutex qmtxLock;
}HIDECODES, *PHIDECODES;

//block response with some file type
typedef struct{
    bool bScript;	//is script file hided;
    bool bImg;
    bool bCss;
    bool bSwf;
    QMutex qmtxLock;
}BLOCKTYPE, *PBLOCKTYPE;

//breakpoint manager
typedef struct{
    bool bGet;	//set breakpoint to GET request
    bool bPost;	//set breakpoint to POST request
    QMutex qmtxLock;
}BREAKP, *PBREAKP;

//breakpoint message struct
typedef struct{
    QWaitCondition qwaitBlock;
    PDATANODE pdnode;
}BREAKMSG, *PBREAKMSG;


#endif // WDDEF_H
