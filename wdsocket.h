#ifndef WDSOCKET_H
#define WDSOCKET_H

#include <QTcpSocket>
#include "wddef.h"

class WDSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit WDSocket(QObject *parent = 0);
    //self-defined construct
    explicit WDSocket(PDATANODE pdnode, QObject *parent=0 ) ;

    //get request from user agent
    bool mGetRequest() ;
    //send request to web server
    bool mSendRequest() ;
    //receive response from web server
    bool mRecvResponse() ;
    //return response to user agent
    bool mRetResponse() ;

    //backtrack func to receive whole message
    int mBTRece(QTcpSocket *pQTsock, char *&pcBuffer, int iLenTotal, int iLenRecvd, int iMark) ;

private:
    //data
    QTcpSocket msock4Server ;	//tcp socket to connect to web server
    PDATANODE mpdnode ;	//structure of socket stream
signals:
    
public slots:
    
};

#endif // WDSOCKET_H
