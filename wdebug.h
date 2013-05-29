#ifndef WDEBUG_H
#define WDEBUG_H

#include <QMainWindow>
#include "monitor.h"
#include "wdserver.h"
#include "fancytabwidget.h"

namespace Ui {
class wdebug;
}

using namespace Core ;
using namespace Core::Internal ;

class wdebug : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit wdebug(QWidget *parent = 0);
    ~wdebug();
    //create and start server
    bool mStartServer() ;

    // data
    FancyTabWidget * mpFancyTab ;
    WDServer *mpServer ;	// local server
    monitor *pwinMonitor;	//child window : monitor

private:
    Ui::wdebug *ui;
};

#endif // WDEBUG_H
