#include "breakpoint.h"
#include "ui_breakpoint.h"
#include "wdevents.h"
#include "wdglobal.h"

extern WDGlobal globals;

breakpoint::breakpoint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::breakpoint)
{
    ui->setupUi(this);
    //set data model of table view list
    mpstItmModelList = new QStandardItemModel();
    ui->tbwList->setModel(mpstItmModelList);
    mpstItmModelList->setColumnCount(2);
    mpstItmModelList->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Type")));
    mpstItmModelList->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("Url")));
    //*** set column width
    ui->tbwList->setColumnWidth(0, 190);
    ui->tbwList->setColumnWidth(1, 190);
    //*** set table view's style
    ui->tbwList->setSelectionBehavior(QAbstractItemView::SelectRows);
    //set data model of table view head
    mpstItmModelHead = new QStandardItemModel();
    ui->tbwHead->setModel(mpstItmModelHead);
    mpstItmModelHead->setColumnCount(2);
    mpstItmModelHead->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("header name")));
    mpstItmModelHead->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("value")));
    //*** set column width
    ui->tbwHead->setColumnWidth(0, 300);
    ui->tbwHead->setColumnWidth(1, 300);
    //*** set table view's style
    ui->tbwHead->setSelectionBehavior(QAbstractItemView::SelectRows);
    //set data model of table view params
    mpstItmModelParams = new QStandardItemModel();
    ui->tbwParams->setModel(mpstItmModelParams);
    mpstItmModelParams->setColumnCount(2);
    mpstItmModelParams->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("parameter name")));
    mpstItmModelParams->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("value")));
    //*** set column width
    ui->tbwParams->setColumnWidth(0, 300);
    ui->tbwParams->setColumnWidth(1, 300);
    //*** set table view's style
    ui->tbwParams->setSelectionBehavior(QAbstractItemView::SelectRows);
}

breakpoint::~breakpoint()
{
    delete ui;
}

//operate msg
void breakpoint::customEvent(QEvent *pevnt)
{
    int iType = pevnt->type();
    if(iType==RUN_TO_BRK)
    {
        EBreakp *pBreakp = (EBreakp*)pevnt;
        mplstMsg.append(pBreakp->mpMsg);
        //diplay to table view list
        //***merge host and url
        QString qstrHost(pBreakp->mpMsg->pdnode->pRequSum->pcHost);
        QString qstrUrl(pBreakp->mpMsg->pdnode->pRequSum->pcUrl);
        QString qstrAbsUrl = qstrHost + qstrUrl;
        qstrAbsUrl.replace(QRegExp("([^:])//"), "\\1/");
        int iIndexNew = mpstItmModelList->rowCount();
        mpstItmModelList->setItem(iIndexNew, 0, new QStandardItem(QString(pBreakp->mpMsg->pdnode->pRequSum->caMethod)));
        mpstItmModelList->setItem(iIndexNew, 1, new QStandardItem(qstrAbsUrl));
    }
}

void breakpoint::on_tbwList_doubleClicked(const QModelIndex &index)
{
    int iIndex =  index.row();
    miIndexSel = iIndex;
    int i = 0;
    //clear old data
    int iCount = mpstItmModelHead->rowCount();
    mpstItmModelHead->removeRows(0, iCount);
    iCount = mpstItmModelParams->rowCount();
    mpstItmModelParams->removeRows(0, iCount);
    //display request header and entity to table view head or params
    QString qstrRawRequ = mplstMsg[iIndex]->pdnode->pcRequRaw;
    QStringList qslstItems = qstrRawRequ.split("\r\n");
    QRegExp qreLineSplit("^([^:]+):\\s+(.+)$");
    mqstrRawReqLine = qslstItems[0];
    for(i=1; i<qslstItems.size(); i++)
    {
        //entity below
        if(qslstItems[i]=="")
            break;
        //headers
        if(qreLineSplit.indexIn(qslstItems[i])!=-1)
        {
            int iIndex = mpstItmModelHead->rowCount();
            mpstItmModelHead->setItem(iIndex, 0, new QStandardItem(qreLineSplit.cap(1)));
            mpstItmModelHead->setItem(iIndex, 1, new QStandardItem(qreLineSplit.cap(2)));
        }
    }
    //analyze entity
    if(i>0 && i+1<qslstItems.size())
    {
        for(i=i+1; i<qslstItems.size(); i++)
        {
            QStringList qslstPairs = qslstItems[i].split('&');
            for(int j=0; j<qslstPairs.size(); j++)
            {
                int iIndex = mpstItmModelParams->rowCount();
                //*** no name-value pair in
                //*** remember unknown
                if(qslstPairs[j].indexOf("=")==-1)
                {
                    mpstItmModelParams->setItem(iIndex, 0, new QStandardItem(qslstPairs[j]));
                    mpstItmModelParams->setItem(iIndex, 1, new QStandardItem("_UNKNOWN"));
                }
                else
                {
                    QStringList qslstNameVal = qslstPairs[j].split('=');
                    mpstItmModelParams->setItem(iIndex, 0, new QStandardItem(qslstNameVal[0]));
                    if(qslstNameVal.size()==2)
                        mpstItmModelParams->setItem(iIndex, 1, new QStandardItem(qslstNameVal[1]));
                }
            }
        }
    }
}

//set breakpoint to POST request
void breakpoint::on_cboxPost_stateChanged(int arg1)
{
     //checked
     if(arg1==2)
     {
        globals.mpBreakPoint->qmtxLock.lock();
        globals.mpBreakPoint->bPost = true;
        globals.mpBreakPoint->qmtxLock.unlock();
     }
     //unchecked
     else if(arg1==0)
     {
        globals.mpBreakPoint->qmtxLock.lock();
        globals.mpBreakPoint->bPost = false;
        globals.mpBreakPoint->qmtxLock.unlock();
     }
}

//continue to run the request
void breakpoint::on_btnRunSel_clicked(bool checked)
{
    //construct
    if(miIndexSel!=-1)
    {
        char *pcNewRaw = NULL;
        int iLenIndex = -1;
        //*** headers
        QStringList qstrlstHeaders;
        for(int i=0; i<mpstItmModelHead->rowCount(); i++)
        {
            if(mpstItmModelHead->item(i,0)->text()=="Content-Length")
                iLenIndex = i;
            QString qstrItem = mpstItmModelHead->item(i,0)->text() + ": " \
                 + mpstItmModelHead->item(i,1)->text();
            qstrlstHeaders.append(qstrItem);
        }
        //*** params
        QStringList qstrlstParams;
        for(int i=0; i<mpstItmModelParams->rowCount(); i++)
        {
            QString qstrItem;
            if(mpstItmModelParams->item(i,1)->text()!="_UNKNOWN")
                qstrItem = mpstItmModelParams->item(i,0)->text() + "=" \
                    + mpstItmModelParams->item(i,1)->text();
            else
                qstrItem = qstrItem = mpstItmModelParams->item(i,0)->text();
            qstrlstParams.append(qstrItem);
        }
        QString qstrParams = qstrlstParams.join("&");
        //merge fields
        if(iLenIndex!=-1)
        {
            //*** new Content-Length
            QString qstrTemp;
            qstrlstHeaders[iLenIndex] = "Content-Length: " + qstrTemp.setNum(qstrParams.length());
            QString qstrHeaders = qstrlstHeaders.join("\r\n");
            //*** new raw request
            int iPosTemp = 0;
            int iLenNew = mqstrRawReqLine.length()+2+qstrHeaders.length()+5+qstrParams.length();
            pcNewRaw = new char[iLenNew];
            qstrcpy(pcNewRaw, mqstrRawReqLine.toAscii().data());
            iPosTemp += mqstrRawReqLine.length();
            qstrcpy(&pcNewRaw[iPosTemp], "\r\n");
            iPosTemp += 2;
            qstrcpy(&pcNewRaw[iPosTemp], qstrHeaders.toAscii().data());
            iPosTemp += qstrHeaders.length();
            qstrcpy(&pcNewRaw[iPosTemp], "\r\n\r\n");
            iPosTemp += 4;
            qstrcpy(&pcNewRaw[iPosTemp], qstrParams.toAscii().data());
            char *p2Del = mplstMsg[miIndexSel]->pdnode->pcRequRaw;
            delete[] p2Del;
            mplstMsg[miIndexSel]->pdnode->pcRequRaw = pcNewRaw;
            mplstMsg[miIndexSel]->pdnode->iLenRequ = iLenNew;
        }
        //run it
        mplstMsg[miIndexSel]->qwaitBlock.wakeAll();
        //clear old data
        int iCount = mpstItmModelHead->rowCount();
        mpstItmModelHead->removeRows(0, iCount);
        iCount = mpstItmModelParams->rowCount();
        mpstItmModelParams->removeRows(0, iCount);
        mpstItmModelList->removeRow(miIndexSel);
        mplstMsg.removeAt(miIndexSel);
        miIndexSel = -1;
    }
}

void breakpoint::on_btnRunAll_clicked()
{
    //clear old data
    int iCount = mpstItmModelHead->rowCount();
    mpstItmModelHead->removeRows(0, iCount);
    iCount = mpstItmModelParams->rowCount();
    mpstItmModelParams->removeRows(0, iCount);
    for(int i=0; i<mplstMsg.size(); i++)
    {
        mplstMsg[i]->qwaitBlock.wakeAll();
        mpstItmModelList->removeRow(i);
        mplstMsg.removeAt(i);
    }
    miIndexSel = -1;
}
