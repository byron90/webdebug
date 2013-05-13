#ifndef WDSERVER_H
#define WDSERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include "wdworker.h"

class WDServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit WDServer(QObject *parent = 0);
    ~WDServer() ;

    // begin listening on port 35791
    bool mBeginListen() ;
    // start monitoring network data flow
    bool mStartAgent() ;
    // pause monitoring network data flow
    bool mPauseAgent() ;

    //data member
    QThreadPool * mpThrdPool ;		//thread pool class

protected:
    virtual void incomingConnection(int socketDescriptor) ;

signals:

public slots:
    
};

#endif // WDSERVER_H
