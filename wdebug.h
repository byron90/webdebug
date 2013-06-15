#ifndef WDEBUG_H
#define WDEBUG_H

#include <QMainWindow>
#include "monitor.h"
#include "wdserver.h"
#include "fancytabwidget.h"
#include "composer.h"
#include "filter.h"
#include "breakpoint.h"

namespace Ui {
class wdebug;
}

using namespace Core;
using namespace Core::Internal;

class wdebug : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit wdebug(QWidget *parent = 0);
    ~wdebug();
    //create and start server
    bool mStartServer();

    // data
    FancyTabWidget * mpFancyTab;
    WDServer *mpServer;	// local server
    monitor *pwinMonitor;	//child window : monitor
    composer *pwinComposer;	//child window: composer
    filter *pwinFilter;	//child window: filter
    breakpoint *pwinBreakPnt; //child window: breakpoint

private slots:
    void switchAgent();

private:
    Ui::wdebug *ui;
};

#endif // WDEBUG_H
