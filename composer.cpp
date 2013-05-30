#include <QApplication>
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
#include <QRegExp>
#include "composer.h"
#include "ui_composer.h"
#include "wdevents.h"
#include "wdebug.h"

extern wdebug *pwdMainWin ;

composer::composer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::composer)
{
    ui->setupUi(this);
    //init combox listing HTTP methods
    QStringList qslMethods;
    qslMethods<<"GET"<<"POST"<<"CONNECT"<<"OPTIONS";
    qslMethods<<"HEAD"<<"PUT"<<"DELETE"<<"TRACE";
    ui->cboxMethod->addItems(qslMethods);
    //init combox listing user-agent strings
    QStringList qslUA;
    //***self-made UA
    qslUA<<"webdebug";
    //***chrome's UA
    qslUA<<"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/27.0.1453.94 Safari/537.36";
    //***firefox's UA
    qslUA<<"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:20.0) Gecko/20100101 Firefox/20.0";
    //***IE's UA
    qslUA<<"Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; Win64; x64; Trident/6.0)";
    ui->cboxUA->addItems(qslUA);
}

composer::~composer()
{
    delete ui;
}

//generate raw request message
void composer::on_btnGenerate_clicked()
{
    //check if lack of some fields
    //***host field
    if(ui->ledtHost->text()=="")
    {
        QMessageBox::information(NULL, "Note", "Host field can't be empty!");
        return;
    }
    //***url field
    if(ui->ledtUrl->text()=="")
        ui->ledtUrl->setText("/");
    else if(!ui->ledtUrl->text().startsWith("/"))
        ui->ledtUrl->setText("/" + ui->ledtUrl->text());
    //generate request
    QString qstrRequest;
    //***start line
    qstrRequest += ui->cboxMethod->currentText() + " " + ui->ledtHost->text()\
                + ui->ledtUrl->text() + " HTTP/1.1\n";
    //***headers
    qstrRequest += "Host: " + ui->ledtHost->text() + "\n";
    if(ui->cboxUA->currentText()!="")
        qstrRequest += "User-Agent: " + ui->cboxUA->currentText() + "\n";
    if(ui->ledtAcpEncode->text()!="")
        qstrRequest += "Accept-Encoding: " + ui->ledtAcpEncode->text() + "\n";
    if(ui->ledtAcpLan->text()!="")
        qstrRequest += "Accept-Language: " + ui->ledtAcpLan->text() + "\n";
            if(ui->ledtRef->text()!="")
        qstrRequest += "Referer: " + ui->ledtRef->text() + "\n";
    //******cookies
    if(ui->tedtCookie->toPlainText()!="")
        qstrRequest += "Cookie: " + ui->tedtCookie->toPlainText() + "\n";
    //******end of header
    qstrRequest += "\n" ;
    ui->tedtPreview->setPlainText(qstrRequest);
}

//send self-made request message
void composer::on_btnSend_clicked()
{
    //move communication to thread
    mqstrRequ = new QString(ui->tedtPreview->toPlainText());
    CommThrd *pcthrdWorker = new CommThrd(mqstrRequ);
    pcthrdWorker->start();
}

//custom event handler
//display response message on text editor
void composer::customEvent(QEvent *pevnt)
{
    int iType = pevnt->type() ;
    if(iType==RESP_AVAIL_COMP)
    {//response available
        ui->tedtResp->setPlainText(*mqstrRequ);
    }
}

//worker in thread
void CommThrd::run()
{
    QTcpSocket qtcpSelfClient;
    QHostAddress qhostLocal("127.0.0.1");
    qtcpSelfClient.connectToHost(qhostLocal, 35791);
    //connect successfully
    if(qtcpSelfClient.waitForConnected())
    {
        //send request message !!!need to be modified
        //replace \n to \r\n
        QRegExp qreTemp("\\n");
        mpqstrRequ->replace(qreTemp, "\r\n");
        QByteArray qbyaRequ = mpqstrRequ->toAscii();
        int i=qtcpSelfClient.write(qbyaRequ);
        if(i==-1)
            return;
            //QMessageBox::information(this, "Composer Error", "Can't send request to localhost!");
        qtcpSelfClient.waitForBytesWritten();
        //read response
        int iAvail = 0;
        QByteArray qbyaResp;
        bool bReady = qtcpSelfClient.waitForReadyRead(30000);
        iAvail = qtcpSelfClient.bytesAvailable();
        qbyaResp = qtcpSelfClient.read(iAvail);
        delete mpqstrRequ;
        mpqstrRequ = new QString(qbyaResp);
        //post message to UI
        ERespAvail *pclsEvnt = new ERespAvail() ;
        qApp->postEvent((QObject*)(pwdMainWin->pwinComposer), pclsEvnt);
    }
        //QMessageBox::information(this, "Composer Error", "Can't connect to localhost!");
}
