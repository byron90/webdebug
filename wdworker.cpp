#include "wdworker.h"
#include <QMessageBox>
#include "wddatar.h"
#include "wddef.h"

extern WDDatar datar ;

WDWorker::WDWorker(int iSocketDes)
{
    miSocket = iSocketDes ;
}

void WDWorker::run()
{
    mpSocket = new WDSocket() ;
    mpSocket->setSocketDescriptor(miSocket) ;
    //read request package
    char * pcRequRaw=NULL ;
    mpSocket->mGetRequest(pcRequRaw) ;
    PDATANODE pdnode = datar.mAddNewRequ(pcRequRaw) ; //save it
    //send the request to web server
    mpSocket->mSendRequest(pdnode) ;
    //receive the response from web server
    char * pcRespRaw ;
    pcRespRaw = mpSocket->mRecvResponse() ;
    datar.mAddNewResp(pcRespRaw) ;	//save it
    //return the response to client
    mpSocket->mRetResponse(pcRespRaw) ;
}
