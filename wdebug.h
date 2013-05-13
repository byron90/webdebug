#ifndef WDEBUG_H
#define WDEBUG_H

#include <QMainWindow>
#include "monitor.h"
#include "wdserver.h"

namespace Ui {
class wdebug;
}

class wdebug : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit wdebug(QWidget *parent = 0);
    ~wdebug();
    // data
    WDServer * mpServer ;	// local server

private slots:
    void on_pushButton_2_clicked();

private:
    monitor * pwinMonitor;
    Ui::wdebug *ui;
};

#endif // WDEBUG_H
