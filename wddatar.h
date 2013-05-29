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
public:
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
    bool mAddNewRequ(PDATANODE pdnode) ;
    //add a new server response to link
    bool mAddNewResp(PDATANODE pdnode) ;
    //analyze raw client request
    bool mAnalyRequ(PDATANODE pdnode) ;
    //save one dnode to list
    int mSaveOne(PDATANODE pdnode) ;
    //analyze raw server response
    bool mAnalyResp(PDATANODE pdnode) ;

};

#endif // WDDATAR_H
