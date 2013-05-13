#ifndef WDSOCKET_H
#define WDSOCKET_H

#include <QTcpSocket>
#include "wddef.h"

class WDSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit WDSocket(QObject *parent = 0);

    //get request from user agent
    void mGetRequest(PDATANODE& pdnode) ;
    //send request to web server
    bool mSendRequest(PDATANODE  pdnodeRequ) ;
    //receive response from web server
    char * mRecvResponse() ;
    //return response to user agent
    bool mRetResponse(char * pcResponse) ;

private:
    //data
    int miLenRecvdRequ ;	//bytes length received ---request
    int miLenTotalRequ ;	//bytes length total one request
    int miMarkRequ ;		//mark for different backtrack situations ---request
    QTcpSocket msock4Server ;	//tcp socket to connect to web server
signals:
    
public slots:
    
};

#endif // WDSOCKET_H
