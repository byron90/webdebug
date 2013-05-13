#include "subwin.h"
#include "ui_subwin.h"

Subwin::Subwin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Subwin)
{
    ui->setupUi(this);
}

Subwin::~Subwin()
{
    delete ui;
}

void Subwin::on_btnBeginLisn_clicked()
{
    pmTcpServer = new QTcpServer(this);
    if (!pmTcpServer->listen(QHostAddress::LocalHost, 35791)) {
        QMessageBox::critical(this, tr("Web Debugger"),
                                   tr("Unable to start the server: %1.")
                                   .arg(pmTcpServer->errorString()));
        close();
        return;
     }
    connect(pmTcpServer, SIGNAL(newConnection()), this, SLOT(mthListen()));
}

void Subwin::mthListen()
{
    pmTcpSocket = pmTcpServer->nextPendingConnection();
    pmTcpSocket->waitForReadyRead();
    //if(pmTcpSocket->bytesAvailable()>0)
     //   QMessageBox::information(this, tr("Hello"), tr("World"));
    //QDataStream dstrmIn(pmTcpSocket);
    //QFile fl("listen_data");
    //fl.open(QIODevice::WriteOnly);
    //dstrmIn >> fl;
    char * pcData = new char[1024];
    pmTcpSocket->read(pcData, 1024);
    QString str(pcData);
    ui->txtData->append(str);
    //pmTcpSocket->disconnectFromHost();
}

void Subwin::on_btnEndLisn_clicked()
{
    pmTcpSocket->disconnectFromHost();
}
