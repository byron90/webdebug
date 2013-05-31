#include <QFile>
#include <QMessageBox>
#include "wdglobal.h"
#include "dlgblockurl.h"
#include "ui_dlgblockurl.h"

extern WDGlobal globals;

DlgBlockUrl::DlgBlockUrl(QWidget *parent):
    QDialog(parent),
    ui(new Ui::DlgBlockUrl)
{
    ui->setupUi(this);
    //init table view
    mpstItmModel = new QStandardItemModel();
    ui->tbwBlockUrl->setModel(mpstItmModel);
    mpstItmModel->setColumnCount(1);
    mpstItmModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Url")));
    ui->tbwBlockUrl->setColumnWidth(0, 400);
    //set table view's style
    ui->tbwBlockUrl->setSelectionBehavior(QAbstractItemView::SelectRows);
    //load url list
    QStringList *pqslstUrl = &globals.mpBlockUrl->qslstUrl;
    for(int i=0; i<pqslstUrl->size(); i++)
        mpstItmModel->setItem(i, 0, new QStandardItem(*pqslstUrls[i]));
}

DlgBlockUrl::~DlgBlockUrl()
{
    //save url list
   // QFile qdocUrls(mqstrFilePath);
   // if(qdocUrls.open(QIODevice::WriteOnly))
   // {
   //     QStringList qslstUrls;
   //     for(int i=0; i<mpstItmModel->rowCount(); i++)
   //         qslstUrls<<mpstItmModel->item(i, 0)->text();
   //     qdocUrls.write(qslstUrls.join("\n").toAscii());
   //     qdocUrls.close();
   // }
   // else
   //     QMessageBox::information(this, "Manage Block Urls", "Can't save url list!");
    delete ui;
}

//select a url to delete
void DlgBlockUrl::on_tbwBlockUrl_doubleClicked(const QModelIndex &index)
{
    QString qstrUrlSel = mpstItmModel->item(index.row(), 0).text();
    ui->ledtUrl.setText(qstrUrlSel);
    miIndexSel = index.row();
}

//delete a url
void DlgBlockUrl::on_btnDeleteUrl_clicked()
{
    if(ui->ledtUrl->text()!=NULL)
    {
        //delete from url list
        QStringList *pqslstUrl = &globals.mpBlockUrl->qslstUrl;
        int iIndex = pqslstUrl->indexOf(ui->ledtUrl->text());
        if(iIndex>-1)
        {
            globals.mpBlockUrl->qmtxLock.lock();
            pqslstUrl->removeAt(iIndex);
            globals.mpBlockUrl->qmtxLock.unlock();
            //remove from table view
            mpstItmModel->removeRow(miIndexSel);
        }
    }
    else
        QMessageBox::information(this, "Manage Block Urls", "Please select a url to delete");

}
