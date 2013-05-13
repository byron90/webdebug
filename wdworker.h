#ifndef WDWORKER_H
#define WDWORKER_H

#include <QRunnable>
#include "wdsocket.h"

class WDWorker : public QRunnable
{
    //Q_OBJECT  !!! not a meta-object
public:

    //data
    WDSocket * mpSocket ;
    //construct with a socket descriptor
    explicit WDWorker(int iSocketDes) ;

private:
    //data
    int miSocket ;
    //core method
    void run() ;
    
};

#endif // WDWORKER_H
