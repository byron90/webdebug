#ifndef DLGBLOCKURL_H
#define DLGBLOCKURL_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DlgBlockUrl;
}

class DlgBlockUrl : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgBlockUrl(QWidget *parent = 0);
    ~DlgBlockUrl();
    

private slots:
    void on_tbwBlockUrl_doubleClicked(const QModelIndex &index);

    void on_btnDeleteUrl_clicked();

private:
    //data
    int miIndexSel;
    QStandardItemModel *mpstItmModel;

    Ui::DlgBlockUrl *ui;
};

#endif // DLGBLOCKURL_H
