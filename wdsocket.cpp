#include <QHostInfo>
#include <string.h>
#include <QDateTime>
#include "wdsocket.h"
#include "wdglobal.h"

extern WDGlobal globals;

WDSocket::WDSocket(QObject *parent) :
    QTcpSocket(parent)
{
}

//self-defined construct
WDSocket::WDSocket(PDATANODE pdnode,QObject *parent):
    QTcpSocket(parent)
{
    //initialize vars
    mpdnode = pdnode ;
}

//backtrack func
//receive the whole http request/response message
//args:  pcBuffer-->data buffer
//		 iLenTotal-->total length of bytes in this request/reponse
//		 iLenRecvd-->length of bytes received
//		 iMark-->used as mark
int WDSocket::mBTRece(QTcpSocket *pQTsocket, char *&pcBuffer, int iLenTotal, int iLenRecvd, int iMark)
{
    if(iMark!=2)
    {
        int iLength = 0 ;
        pQTsocket->waitForReadyRead() ;
        iLength = pQTsocket->bytesAvailable() ;
        if(iLength<=0)
            return -1;
        if(iMark==0)
        {//new request
            iLenRecvd += iLength ;
            pcBuffer = new char[iLength+1] ;
            pQTsocket->read(pcBuffer, iLength) ;
        }
        else
        {//backtrack: no empty line in last call
            char * pcLast = pcBuffer ;
            pcBuffer = new char[iLength+iLenRecvd+1] ;
            memcpy(pcBuffer, pcLast, iLenRecvd) ;
            pQTsocket->read(&pcBuffer[iLenRecvd], iLength) ;
            iLenRecvd += iLength ;
            delete[] pcLast ;
        }
        //calculate the length of this request
        char * pcRequEnd = strstr(pcBuffer, "\r\n\r\n") ;
        if(pcRequEnd)
        {//request has only one tcp packet, maybe.....
            //check if a message-body in this request
            char * pcHeaderLen = NULL ;
            pcHeaderLen = strstr(pcBuffer, "Content-Length:") ;
            if(!pcHeaderLen)
            {
                pcBuffer[iLenRecvd] = '\0' ;
                return iLenRecvd ;		//has no message-body
            }
            else if(pcHeaderLen<pcRequEnd)
            {//Content-Length header not in message-body
                char caLength[10] ;
                int iCur = 0 ;
                //get content length value
                int iContentLen ;
                while(*pcHeaderLen!='\r')
                {
                    if(*pcHeaderLen>='0' && *pcHeaderLen<='9')
                    {
                        caLength[iCur] = *pcHeaderLen ;
                        iCur++ ;
                    }
                    pcHeaderLen++ ;
                }
                caLength[iCur] = '\0' ;
                iContentLen = QString(caLength).toLong() ;
                iLenTotal = iContentLen ;
                //check if the whole message-body is in this packet
                if(iContentLen == (iLenRecvd-(pcRequEnd+4-pcBuffer)))
                {
                    pcBuffer[iLenRecvd] = '\0' ;
                    return iLenRecvd ;	//yes, whole message-body is here
                }
                else
                {//no, message-body is in diff packet
                    iMark = 2 ;
                    iLenTotal = pcRequEnd + 4 - pcBuffer + iContentLen ;
                    return mBTRece(pQTsocket, pcBuffer, iLenTotal, iLenRecvd, iMark);
                }
            }
            else
            {//string 'Content-Length' is in message body
                pcBuffer[iLenRecvd] = '\0' ;
                return iLenRecvd ;
            }
        }
        else
        {//request has two tcp packets at least
            iMark = 1 ;
            return mBTRece(pQTsocket, pcBuffer, iLenTotal, iLenRecvd, iMark);
        }
    }
    else
    {// message-body is in different packets
        int iLength = 0 ;
        char * pcLast ;
        pQTsocket->waitForReadyRead() ;
        iLength = pQTsocket->bytesAvailable() ;
        pcLast = pcBuffer ;
        pcBuffer = new char[iLength+iLenRecvd+1] ;
        memcpy(pcBuffer, pcLast, iLenRecvd) ;
        pQTsocket->read(&pcBuffer[iLenRecvd], iLength) ;
        iLenRecvd += iLength ;
        delete[] pcLast ;
        if(iLenRecvd==iLenTotal)
        {
            pcBuffer[iLenRecvd] = '\0' ;
            return iLenTotal;
        }
        else
        {//still has more packets
            return mBTRece(pQTsocket, pcBuffer, iLenTotal, iLenRecvd, iMark);
        }
    }
}

//get request from user agent
//backtrack func
//main operation is checking multiful packets in one request
bool WDSocket::mGetRequest()
{
    int iLenTotal = 0 ;	//total length of request message
    int iLenRecvd = 0 ; //length of received bytes
    int iMark = 0 ;		//to mark the backtrack state
    mpdnode->iLenRequ = mBTRece(this, mpdnode->pcRequRaw, iLenTotal, iLenRecvd, iMark) ;
    if(mpdnode->iLenRequ<0)
    {
        //write log
        QString qstrLog = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        qstrLog += " getRequest: error return -1";
        globals.writeLog(qstrLog);
        return false;
    }
    //write log
    QString qstrLog = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrLog += " getRequest: ok";
    globals.writeLog(qstrLog);
    return true ;
}

// send request to web server
bool WDSocket::mSendRequest()
{
    if(mpdnode->pRequSum->pcHost)
    {//host is valid
        msock4Server.connectToHost(mpdnode->pRequSum->pcHost, 80);
        //try to connect to web server
        //if the web server can not conected after 3 times
        //then return false
        int iTime = 0 ;
        while(!msock4Server.waitForConnected() && iTime<4)
        {
            iTime++ ;
        }
        if(iTime<4)
        {//connected
            //forwarding client's request to origin server
            msock4Server.write(mpdnode->pcRequRaw, mpdnode->iLenRequ) ;
        }
        else
            //can't connect to server
            return false ;
    }
    else
        return false ;
}

// receive response from web server
bool WDSocket::mRecvResponse()
{
    int iLenTotal = 0 ;	//total length of response message
    int iLenRecvd = 0 ; //length of received bytes
    int iMark = 0 ;		//to mark the backtrack state
    mpdnode->iLenResp = mBTRece(&msock4Server, mpdnode->pcRespRaw, iLenTotal, iLenRecvd, iMark) ;
    if(mpdnode->iLenResp<0)
    {
        QString qstrLog = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        qstrLog += " recvResp: error return -1";
        globals.writeLog(qstrLog);
        return false ;
    }
    QString qstrLog = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrLog += " recvResp: error return -1";
    globals.writeLog(qstrLog);
    return true ;
}

// return response to user agent
bool WDSocket::mRetResponse()
{
    write(mpdnode->pcRespRaw, mpdnode->iLenResp) ;
    waitForBytesWritten(30000);
    QString qstrLog = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrLog += " returnResp: ok";
    globals.writeLog(qstrLog);
    return true ;
}
