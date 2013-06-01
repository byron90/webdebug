#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include "filter.h"
#include "ui_filter.h"
#include "dlgblockurl.h"
#include "dlgautoresp.h"
#include "wdevents.h"

extern WDGlobal globals;

filter::filter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::filter)
{
    ui->setupUi(this);
    //init filter type list
    mqslTypes<<"BlockUrl";	mqslTypes<<"AutoResponse";
    mqslTypes<<"HideCodes";	mqslTypes<<"BlockType";
    //set data model of table view
    mpstItmModel = new QStandardItemModel();
    ui->tbwFilters->setModel(mpstItmModel);
    mpstItmModel->setColumnCount(3);
    mpstItmModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Type")));
    mpstItmModel->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("Url")));
    mpstItmModel->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("Note")));
    //set column width
    ui->tbwFilters->setColumnWidth(0, 100);
    ui->tbwFilters->setColumnWidth(1, 450);
    ui->tbwFilters->setColumnWidth(2, 450);
    //set table view's style
    ui->tbwFilters->setSelectionBehavior(QAbstractItemView::SelectRows);
}

filter::~filter()
{
    delete ui;
}

//custom event handlering center
void filter::customEvent(QEvent *pevnt)
{
    int iType = pevnt->type() ;
    if(iType==NEW_HIT_FLT)
    {//new hit available
        EFHits *pevntNewHit = (EFHits *)pevnt ;
        int iRowCount = mpstItmModel->rowCount() ;
        QString strTemp ;
        //add a row to item model containing info about a hit
        mpstItmModel->setItem(iRowCount, 0, new QStandardItem(mqslTypes[pevntNewHit->miType]));	//type
        mpstItmModel->setItem(iRowCount, 1, new QStandardItem(pevntNewHit->mqstrUrl));		//url
        mpstItmModel->setItem(iRowCount, 2, new QStandardItem(pevntNewHit->mqstrNote));		//note
    }
    //delete pevnt;
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
            QMessageBox::information(this, "Filter BlockUrl", "Already have it!");
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
        QMessageBox::information(this, "Filter BlockUrl", "Add successfully.");
    }
    else
        QMessageBox::information(this, "Filter BlockUrl", "URL is empty!");
}

//display dialog to check and delete block url
void filter::on_btnBkUrlDetl_clicked()
{
    DlgBlockUrl dlgBlockUrl(this);
    dlgBlockUrl.exec();
}

//display file dialog to select source file
void filter::on_btnAutoPath_clicked()
{
    mqstrSrcPath = QFileDialog::getOpenFileName(this, "select source file");
    ui->ledtSrcPath->setText(mqstrSrcPath);
}

//
void filter::on_btnAddAutoResp_clicked()
{
    QString qstrNewUrl = ui->ledtAuto->text();
    if(qstrNewUrl!=NULL && mqstrSrcPath!=NULL)
    {
        QList<AUTORNODE> *plstNodes = &(globals.mpAutoResp->qlstNodes);
        //lock and save
        globals.mpAutoResp->qmtxLock.lock();
        //***have a check
        AUTORNODE stAutoRespTemp;
        stAutoRespTemp.qstrUrl = qstrNewUrl;
        stAutoRespTemp.qstrSrcPath = mqstrSrcPath;
        int iAdded = plstNodes->indexOf(stAutoRespTemp);
        if(iAdded>-1)
        {
            globals.mpAutoResp->qmtxLock.unlock();
            QMessageBox::information(this, "Filter AutoResponse", "Already have it!");
            return;
        }
        plstNodes->append(stAutoRespTemp);
        //***save to file
        QFile qdocTemp(globals.mpAutoResp->qstrFilePath);
        qdocTemp.open(QIODevice::WriteOnly);
        //!!!todos: separator changes with system
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
        globals.mpAutoResp->qmtxLock.unlock();
        QMessageBox::information(this, "Filter AutoResponse", "Add successfully.");
    }
    else
        QMessageBox::information(this, "Filter AutoResponse", "Both url and source file are need!");
}

//display dialog to check and delete auto response item
void filter::on_btnAutoDetl_clicked()
{
    DlgAutoResp dlgAutoResp(this);
    dlgAutoResp.exec();
}

//hide 2** check box
void filter::on_cboxHide2_stateChanged(int arg1)
{
    //checked
    if(arg1==2)
    {
        globals.mpHideCodes->qmtxLock.lock();
        globals.mpHideCodes->b2 = true;
        globals.mpHideCodes->qmtxLock.unlock();
    }
    //unchecked
    else if(arg1==0)
    {
        globals.mpHideCodes->qmtxLock.lock();
        globals.mpHideCodes->b2 = false;
        globals.mpHideCodes->qmtxLock.unlock();
    }
}

//hide 304 check box
void filter::on_cboxHide304_stateChanged(int arg1)
{
    //checked
    if(arg1==2)
    {
        globals.mpHideCodes->qmtxLock.lock();
        globals.mpHideCodes->b304 = true;
        globals.mpHideCodes->qmtxLock.unlock();
    }
    //unchecked
    else if(arg1==0)
    {
        globals.mpHideCodes->qmtxLock.lock();
        globals.mpHideCodes->b304 = false;
        globals.mpHideCodes->qmtxLock.unlock();
    }
}

//hide 302 check box
void filter::on_cboxHide302_stateChanged(int arg1)
{
    //checked
    if(arg1==2)
    {
        globals.mpHideCodes->qmtxLock.lock();
        globals.mpHideCodes->b302 = true;
        globals.mpHideCodes->qmtxLock.unlock();
    }
    //unchecked
    else if(arg1==0)
    {
        globals.mpHideCodes->qmtxLock.lock();
        globals.mpHideCodes->b302 = false;
        globals.mpHideCodes->qmtxLock.unlock();
    }
}

//hide not 2** check box
void filter::on_cboxHiden2_stateChanged(int arg1)
{
    //checked
    if(arg1==2)
    {
        globals.mpHideCodes->qmtxLock.lock();
        globals.mpHideCodes->bnot2 = true;
        globals.mpHideCodes->qmtxLock.unlock();
    }
    //unchecked
    else if(arg1==0)
    {
        globals.mpHideCodes->qmtxLock.lock();
        globals.mpHideCodes->bnot2 = false;
        globals.mpHideCodes->qmtxLock.unlock();
    }
}

//block script check box
void filter::on_cboxScript_stateChanged(int arg1)
{
    //checked
    if(arg1==2)
    {
        globals.mpBlockType->qmtxLock.lock();
        globals.mpBlockType->bScript = true;
        globals.mpBlockType->qmtxLock.unlock();
    }
    //unchecked
    else if(arg1==0)
    {
        globals.mpBlockType->qmtxLock.lock();
        globals.mpBlockType->bScript = false;
        globals.mpBlockType->qmtxLock.unlock();
    }
}

//block img check box
void filter::on_cboxImg_stateChanged(int arg1)
{
    //checked
    if(arg1==2)
    {
        globals.mpBlockType->qmtxLock.lock();
        globals.mpBlockType->bImg = true;
        globals.mpBlockType->qmtxLock.unlock();
    }
    //unchecked
    else if(arg1==0)
    {
        globals.mpBlockType->qmtxLock.lock();
        globals.mpBlockType->bImg = false;
        globals.mpBlockType->qmtxLock.unlock();
    }
}

//block css check box
void filter::on_cboxCSS_stateChanged(int arg1)
{
    //checked
    if(arg1==2)
    {
        globals.mpBlockType->qmtxLock.lock();
        globals.mpBlockType->bCss = true;
        globals.mpBlockType->qmtxLock.unlock();
    }
    //unchecked
    else if(arg1==0)
    {
        globals.mpBlockType->qmtxLock.lock();
        globals.mpBlockType->bCss = false;
        globals.mpBlockType->qmtxLock.unlock();
    }
}

//block swf check box
void filter::on_cboxSwf_stateChanged(int arg1)
{
    //checked
    if(arg1==2)
    {
        globals.mpBlockType->qmtxLock.lock();
        globals.mpBlockType->bSwf = true;
        globals.mpBlockType->qmtxLock.unlock();
    }
    //unchecked
    else if(arg1==0)
    {
        globals.mpBlockType->qmtxLock.lock();
        globals.mpBlockType->bSwf = false;
        globals.mpBlockType->qmtxLock.unlock();
    }

}
