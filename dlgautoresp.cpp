#include <QFile>
#include <QMessageBox>
#include "wdglobal.h"
#include "dlgautoresp.h"
#include "ui_dlgautoresp.h"

extern WDGlobal globals;

DlgAutoResp::DlgAutoResp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAutoResp)
{
    ui->setupUi(this);
    //init table view
    mpstItmModel = new QStandardItemModel();
    ui->tbwAutoResps->setModel(mpstItmModel);
    mpstItmModel->setColumnCount(2);
    mpstItmModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Url")));
    ui->tbwAutoResps->setColumnWidth(0, 300);
    mpstItmModel->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("Source file")));
    ui->tbwAutoResps->setColumnWidth(1, 300);
    //set table view's style
    ui->tbwAutoResps->setSelectionBehavior(QAbstractItemView::SelectRows);
    //load AutoResponse list
    QList<AUTORNODE> *plstNodes = &(globals.mpAutoResp->qlstNodes);
    for(int i=0; i<plstNodes->count(); i++)
    {
        mpstItmModel->setItem(i, 0, new QStandardItem((*plstNodes)[i].qstrUrl));
        mpstItmModel->setItem(i, 1, new QStandardItem((*plstNodes)[i].qstrSrcPath));
    }
}

DlgAutoResp::~DlgAutoResp()
{
    //save items in list to file
    QList<AUTORNODE> *plstNodes = &(globals.mpAutoResp->qlstNodes);
    QFile qdocTemp(globals.mpAutoResp->qstrFilePath);
    qdocTemp.open(QIODevice::WriteOnly);
    QStringList qslstData2Save;
    for(int i=0; i<plstNodes->count(); i++)
    {
        QString qstrOne;
        qstrOne += (*plstNodes)[i].qstrUrl + "\t";
        qstrOne += (*plstNodes)[i].qstrSrcPath;
        qslstData2Save<<qstrOne;
    }
    QString qstrData2Save = qslstData2Save.join("\n");
    qdocTemp.write(qstrData2Save.toAscii());
    qdocTemp.close();
    delete ui;
}

//double click table view's row
void DlgAutoResp::on_tbwAutoResps_doubleClicked(const QModelIndex &index)
{
    miIndexSel = index.row();
    ui->ledtUrl->setText(mpstItmModel->item(miIndexSel, 0)->text());
    ui->ledtSrc->setText(mpstItmModel->item(miIndexSel, 1)->text());
}

//delete selected auto response item
void DlgAutoResp::on_btnDelete_clicked()
{
    if(ui->ledtUrl->text()!=NULL)
    {
        //delete from item list
        AUTORNODE stItemDel;
        stItemDel.qstrUrl = ui->ledtUrl->text();
        stItemDel.qstrSrcPath = ui->ledtSrc->text();
        int iIndex = globals.mpAutoResp->qlstNodes.indexOf(stItemDel);
        if(iIndex>-1)
        {
            globals.mpAutoResp->qlstNodes.removeAt(iIndex);
            //delete from table view
            mpstItmModel->removeRow(miIndexSel);
        }
    }
    else
        QMessageBox::information(this, "Manage auto response", "Please select an item to delete");
}
