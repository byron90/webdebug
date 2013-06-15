#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <QWidget>
#include <QStandardItemModel>
#include <QList>
#include "wddef.h"

namespace Ui {
class breakpoint;
}

class breakpoint : public QWidget
{
    Q_OBJECT
    
public:
    explicit breakpoint(QWidget *parent = 0);
    ~breakpoint();

protected:
    void customEvent(QEvent *event);
    
private slots:
    void on_tbwList_doubleClicked(const QModelIndex &index);

    void on_cboxPost_stateChanged(int arg1);

    void on_btnRunSel_clicked(bool checked);

private:
    //data
    QStandardItemModel *mpstItmModelHead;	//table head
    QStandardItemModel *mpstItmModelParams;	//table param
    QStandardItemModel *mpstItmModelList;	//table list
    QList<PBREAKMSG> mplstMsg;	//list of visit nodes
    int miIndexSel;	//index of table view list's selected item
    QString mqstrRawReqLine;

    Ui::breakpoint *ui;
};

#endif // BREAKPOINT_H
