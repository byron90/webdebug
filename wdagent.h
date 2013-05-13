#ifndef WDAGENT_H
#define WDAGENT_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
//#include <wdThrdPool>		thread pool class

class wdAgent : public QObject
{
    Q_OBJECT
public:
    explicit wdAgent(QObject *parent = 0);

    //datas			 !!!may be private
    QTcpServer * mpTcpSvr;	//listening server
    QTcpSocket ** mplstSock;	//lists of connect socket

    //funcs
    bool mBeginLisn();		//begin listening
    bool mAgentStart();		//start the agent to monitor data flow
    bool mAgentPause();		//interrupt the agent
    bool mGetRequest();		//get request from user agent
    bool mSendRequest();	//send request to web server
    bool mRecvResponse();	//receive response from web server
    bool mRetResponse();	//return response to user agent


signals:
    
public slots:
    bool mNewConn();		//when new connection created
};

#endif // WDAGENT_H
