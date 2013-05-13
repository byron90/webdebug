#include "wdserver.h"
#include <QMessageBox>

WDServer::WDServer(QObject *parent) :
    QTcpServer(parent)
{
    //init thread pool
    mpThrdPool = NULL ;
    mpThrdPool = new QThreadPool() ;
    mpThrdPool->setExpiryTimeout(10000) ;
    mpThrdPool->setMaxThreadCount(20);
}

WDServer::~WDServer()
{
    //wait for all thread exiting
    if(mpThrdPool)
        mpThrdPool->waitForDone(3000) ;
}

bool WDServer::mBeginListen()
{
        if(!listen(QHostAddress::LocalHost, 35791))
         {
                QMessageBox::critical(NULL, tr("Web Debugger"),
                                           tr("Unable to start the server: %1.").arg(errorString()));
                return false ;
         }
         return true ;
}

// begin a new worker
void WDServer::incomingConnection(int socketDescriptor)
{
    WDWorker * pWorker = new WDWorker(socketDescriptor) ;
    //QMessageBox::information(NULL, "Web Debugger", "new connection") ;
    mpThrdPool->start(pWorker) ;
}

// start monitoring network data flow
bool mStartAgent()
{
    return true ;
}

// pause monitoring network data flow
bool mPauseAgent()
{
    return true ;
}
