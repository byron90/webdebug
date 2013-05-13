#include "wdagent.h"
#include <QMessageBox>

wdAgent::wdAgent(QObject *parent) :
    QObject(parent)
{
    mpTcpSvr = NULL ;
    mplstSock = NULL ;
}

bool wdAgent::mBeginLisn()
{
    // begin to listen on port 35791
    mpTcpSvr = new QTcpServer() ;
    if( !mpTcpSvr->listen(QHostAddress::LocalHost, 35791) )
    {
        QMessageBox::critical( this, "web debugger",
                                    ("Error! Unable to start the server. %1").arg(mpTcpSvr->errorString())) ;
        return  false ;
    }
    connect( mpTcpSvr, SIGNAL(newConnection()), this, SLOT(mNewConn)) ;
    return true ;
}

bool wdAgent::mAgentStart()
{
    //start the monitoring
}

bool wdAgent::mAgentPause()
{
    //interrupt the agent
}

bool wdAgent::mNewConn()
{
    //when new connection created
   pTmpSocket = mpTcpSvr->nextPendingConnection() ;

}

bool wdAgent::mGetRequest()
{
    //get request from user agent
}

bool wdAgent::mSendRequest()
{
    //send request to web server
}

bool wdAgent::mRecvResponse()
{
    //receive response from web server
}

bool wdAgent::mRetResponse()
{
    //return response to user agent
}
