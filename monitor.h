#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSplitter>

namespace Ui {
class monitor;
}

class monitor : public QWidget
{
    Q_OBJECT
    
public:
    explicit monitor(QWidget *parent = 0);
    ~monitor();
    
    //data
    QStandardItemModel *mpstItmModel ;

protected:
    void customEvent(QEvent *event) ;
private slots:
    void on_tbwVisitLst_doubleClicked(const QModelIndex &index);

private:
    Ui::monitor *ui;
};

#endif // MONITOR_H
