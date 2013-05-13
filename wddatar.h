#ifndef WDDATAR_H
#define WDDATAR_H
#include <QList>
#include <QMutexLocker>
#include "wddef.h"

// Data operation layer.
// Provide interface to save and anaylze network data package
// May save 1,000 visit(request and response).

class WDDatar
{
    // datas
    QList<DATANODE> mLstVisit ;	//list of visits(req and resp)
    //QMutexLocker mMtxlocLst ;		//mutex locker for mLstVisit
    //QMutex mMtxRespOp ;		//mutex for response data operation
    QMutex mMtxLst ;	//mutex for  visit list

    // methods
public:
    WDDatar();
    ~WDDatar() ;
    //add a new client request to link
    PDATANODE mAddNewRequ(char * pcRequ) ;
    //add a new server response to link
    bool mAddNewResp(char * pcResp) ;
    //analyze raw client request
    PREQUSUM mAnalyRequ(char * pcRequ) ;
    //analyze raw server response
    //PRESP mAnalyResp(char * pcResp) ;

};

#endif // WDDATAR_H
