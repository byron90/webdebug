#include <QFile>
#include <QMessageBox>
#include "filter.h"
#include "ui_filter.h"
#include "dlgblockurl.h"

extern WDGlobal globals;

filter::filter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::filter)
{
    ui->setupUi(this);
}

filter::~filter()
{
    delete ui;
}

//add a new block url
//save it to file and block url list
void filter::on_btnAddBkUrl_clicked()
{
    QString qstrNewUrl = ui->ledtBkUrl->text();
    if(qstrNewUrl!=NULL)
    {
        //lock and save
        globals.mpBlockUrl->qmtxLock.lock();
        //***have a check
        int iAdded = globals.mpBlockUrl->qslstUrl.indexOf(qstrNewUrl);
        if(iAdded>-1)
        {
            globals.mpBlockUrl->qmtxLock.unlock();
            QMessageBox::information(this, "Filter", "Already have it!");
            return;
        }
        globals.mpBlockUrl->qslstUrl<<qstrNewUrl;
        //***save to file
        QFile qdocTemp(globals.mpBlockUrl->qstrFilePath);
        qdocTemp.open(QIODevice::WriteOnly);
        //!!!todos: separator changes with system
        QString qstrUrls = globals.mpBlockUrl->qslstUrl.join("\n");
        qdocTemp.write(qstrUrls.toAscii());
        qdocTemp.close();
        globals.mpBlockUrl->qmtxLock.unlock();
        QMessageBox::information(this, "Filter", "Add successfully.");
    }
    else
        QMessageBox::information(this, "Filter", "URL is empty!");
}

void filter::on_btnBkUrlDetl_clicked()
{
    DlgBlockUrl dlgBlockUrl(globals.mpBlockUrl->qstrFilePath, this);
    dlgBlockUrl.exec();
}
