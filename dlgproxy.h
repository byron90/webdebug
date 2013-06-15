#ifndef DLGPROXY_H
#define DLGPROXY_H

#include <QDialog>

namespace Ui {
class DlgProxy;
}

class DlgProxy : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgProxy(QWidget *parent = 0);
    ~DlgProxy();
    
private slots:
    void on_btnProxy_clicked();

    void on_btnDisproxy_clicked();

private:
    Ui::DlgProxy *ui;
};

#endif // DLGPROXY_H
