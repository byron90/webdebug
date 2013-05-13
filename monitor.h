#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>

namespace Ui {
class monitor;
}

class monitor : public QWidget
{
    Q_OBJECT
    
public:
    explicit monitor(QWidget *parent = 0);
    ~monitor();
    
private:
    Ui::monitor *ui;
};

#endif // MONITOR_H
