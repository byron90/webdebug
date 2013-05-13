#include "wdsocket.h"
#include <QHostInfo>
#include <string.h>

WDSocket::WDSocket(QObject *parent) :
    QTcpSocket(parent)
{
    //initialize vars
    miLenRecvdRequ = 0 ;
    miLenTotalRequ = 0 ;
    miMarkRequ = 0 ;	//0 means the first packet
                                    //1 means no empty line in request message
                                    //2 means message-body is in different packet
}

//get request from user agent
//backtrack func
//main operation is checking multiful packets in one request
void WDSocket::mGetRequest(PDATANODE &pdnode)
{
    char * pcRequRaw = NULL ;
    if(miMarkRequ!=2)
    {
        int iLength = 0 ;
        waitForReadyRead() ;
        iLength = bytesAvailable() ;
        if(iLength<=0)
            return ;
        if(miMarkRequ==0)
        {//new request
            miLenRecvdRequ += iLength ;
            pcRequRaw = new char[iLength+1] ;
            read(pcRequRaw, iLength) ;
            pcRequRaw[iLength] = '\0' ;
        }
        if(miMarkRequ==1)
        {//backtrack: no empty line in last call
            char * pcLast = pcRequRaw ;
            pcRequRaw = new char[iLength+miLenRecvdRequ+1] ;
            read(&pcRequRaw[miLenRecvdRequ], iLength) ;
            miLenRecvdRequ += iLength ;
            pcRequRaw[miLenRecvdRequ] = '\0' ;
            delete[] pcLast ;
        }
        //calculate the length of this request
        char * pcRequEnd = strstr(pcRequRaw, "\r\n\r\n") ;
        if(pcRequEnd)
        {//request has one tcp packet, maybe.....
            //check if a message-body in this request
            char * pcHeaderLen = NULL ;
            pcHeaderLen = strstr(pcRequRaw, "Content-Length:") ;
            if(!pcHeaderLen)
            {//no Content-Length. packet ends
                pdnode->pcRequRaw = pcRequRaw ;
                pdnode->iLenRequ = miLenRecvdRequ ;
                miMarkRequ = 0 ;
                miLenRecvdRequ = 0 ;
                miLenTotalRequ = 0;
                return ;		//has no message-body
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
                miLenTotalRequ = iContentLen ;
                //check if the whole message-body is in this packet
                if(iContentLen == (miLenRecvdRequ-(pcRequEnd+4-pcRequRaw)))
                {
                    pdnode->pcRequRaw = pcRequRaw ;
                    pdnode->iLenRequ = miLenRecvdRequ ;
                    miMarkRequ = 0 ;
                    miLenRecvdRequ = 0 ;
                    miLenTotalRequ = 0 ;
                    return ;	//yes, whole message-body is here
                }
                else
                {//no, message-body is in diff packet
                    miLenRecvdRequ += iLength ;
                    miMarkRequ = 2 ;
                    mGetRequest(pcRequRaw);
                }
            }
            else
                return ;
        }
        else
        {//request has two tcp packets at least
            miLenRecvdRequ += iLength ;
            miMarkRequ = 1 ;
            mGetRequest(pcRequRaw);
        }
    }
    else
    {// message-body is in different packets
        int iLength = 0 ;
        char * pcLast ;
        waitForReadyRead() ;
        iLength = bytesAvailable() ;
        pcLast = pcRequRaw ;
        pcRequRaw = new char[iLength+miLenRecvdRequ+1] ;
        read(&pcRequRaw[miLenRecvdRequ], iLength) ;
        miLenRecvdRequ += iLength ;
        pcRequRaw[miLenRecvdRequ] = '\0' ;
        delete[] pcLast ;
        if(miLenRecvdRequ==miLenTotalRequ)
        {
            pdnode->pcRequRaw = pcRequRaw ;
            pdnode->iLenRequ = miLenRecvdRequ ;
            miMarkRequ = 0 ;
            miLenRecvdRequ = 0 ;
            miLenTotalRequ = 0 ;
            return ;
        }
        else
        {//still has more packets
            mGetRequest(pcRequRaw);
        }
    }
}

// send request to web server
bool WDSocket::mSendRequest(PDATANODE pdnodeRequ)
{
    if(pdnodeRequ->pRequSum->pcHost)
    {//host is valid
        msock4Server.connectToHost(psumRequ->pcHost, 80);
        //try to connect to web server
        //if the web server can not conected in 3 times
        //then return false
        int iTime = 0 ;
        while(!msock4Server.waitForConnected() && iTime<4)
        {
            iTime++ ;
        }
        if(iTime<4)
        {//connected
            msock4Server.write(pdnodeRequ->pcRequRaw, pdnodeRequ->iLenRequ) ;
        }
        else
            return false ;
    }
    else
        return false ;
}

// receive response from web server
char * WDSocket::mRecvResponse()
{
    return NULL ;
}

// return response to user agent
bool WDSocket::mRetResponse(char * pcResponse)
{
    return true ;
}
