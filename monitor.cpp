#include <QMessageBox>
#include <QSizePolicy>
#include <QList>
#include "monitor.h"
#include "ui_monitor.h"
#include "wddatar.h"
#include "wdevents.h"
#include "wddef.h"

extern WDDatar datar;

monitor::monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::monitor)
{
    ui->setupUi(this);
    //set data model of table view
    mpstItmModel = new QStandardItemModel();
    ui->tbwVisitLst->setModel(mpstItmModel);
    mpstItmModel->setColumnCount(7);
    mpstItmModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Method")));
    mpstItmModel->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("Host")));
    mpstItmModel->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("Url")));
    mpstItmModel->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("Code")));
    mpstItmModel->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("Code Stat")));
    mpstItmModel->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("content type")));
    //set column width
    ui->tbwVisitLst->setColumnWidth(0, 100);
    ui->tbwVisitLst->setColumnWidth(1, 250);
    ui->tbwVisitLst->setColumnWidth(2, 350);
    ui->tbwVisitLst->setColumnWidth(3, 80);
    ui->tbwVisitLst->setColumnWidth(4, 100);
    ui->tbwVisitLst->setColumnWidth(5, 100);
    ui->tbwVisitLst->setColumnHidden(6, true);
    //set table view's style
    ui->tbwVisitLst->setSelectionBehavior(QAbstractItemView::SelectRows);
    //set width of splitter's widget
    QList<int> qlstSizes ;
    qlstSizes.append(910);
    qlstSizes.append(20);
    ui->splitter->setSizes(qlstSizes);
}

monitor::~monitor()
{
    delete ui;
}

//custom event handlering center
void monitor::customEvent(QEvent *pevnt)
{
    int iType = pevnt->type() ;
    if(iType==NEW_VISIT_MON)
    {//new visit available
        ENewVisit *pevntNewVisit = (ENewVisit *)pevnt ;
        int iNumInLst = pevntNewVisit->miPos ;
        int iRowCount = mpstItmModel->rowCount() ;
        QString strTemp ;
        //add a row to item model containing info about a visit
        mpstItmModel->setItem(iRowCount, 0, new QStandardItem(datar.mLstVisit[iNumInLst].pRequSum->caMethod));		//method
        mpstItmModel->setItem(iRowCount, 1, new QStandardItem(datar.mLstVisit[iNumInLst].pRequSum->pcHost));		//host
        mpstItmModel->setItem(iRowCount, 2, new QStandardItem(datar.mLstVisit[iNumInLst].pRequSum->pcUrl));		//url
        mpstItmModel->setItem(iRowCount, 3, new QStandardItem(datar.mLstVisit[iNumInLst].pRespSum->caCode));		//response code
        mpstItmModel->setItem(iRowCount, 4, new QStandardItem(datar.mLstVisit[iNumInLst].pRespSum->caStat));		//response code
        mpstItmModel->setItem(iRowCount, 5, new QStandardItem(datar.mLstVisit[iNumInLst].pRespSum->pcType));		//response code
        mpstItmModel->setItem(iRowCount, 6, new QStandardItem(strTemp.setNum(iNumInLst)));		//response length
    }
    //delete pevnt;
}

void monitor::on_tbwVisitLst_doubleClicked(const QModelIndex &index)
{
    //index the data node in list
    int iPos = mpstItmModel->item(index.row(), 6)->text().toLong() ;
    //load request and response messages to ui
    ui->textEdit->setText(datar.mLstVisit[iPos].pcRequRaw);
    ui->textEdit_2->setText(datar.mLstVisit[iPos].pcRespRaw);

    QList<int> qlstSizes ;
    qlstSizes.append(100);
    qlstSizes.append(800);
    ui->splitter->setSizes(qlstSizes);
}

