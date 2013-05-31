#ifndef DLGAUTORESP_H
#define DLGAUTORESP_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DlgAutoResp;
}

class DlgAutoResp : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgAutoResp(QWidget *parent = 0);
    ~DlgAutoResp();
    
private slots:
    void on_tbwAutoResps_doubleClicked(const QModelIndex &index);

    void on_btnDelete_clicked();

private:
    //data
    int miIndexSel;
    QStandardItemModel *mpstItmModel;

    Ui::DlgAutoResp *ui;
};

#endif // DLGAUTORESP_H
