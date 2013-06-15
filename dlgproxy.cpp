#include <QMessageBox>
#include "dlgproxy.h"
#include "ui_dlgproxy.h"
#include "wdglobal.h"

extern WDGlobal globals;

DlgProxy::DlgProxy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgProxy)
{
    ui->setupUi(this);
}

DlgProxy::~DlgProxy()
{
    delete ui;
}

void DlgProxy::on_btnProxy_clicked()
{
    QString qstrHost = ui->ledtHost->text();
    QString qstrPort = ui->ledtPort->text();
    if(qstrHost!=NULL && qstrPort!=NULL)
    {
        globals.mpProxyConf->qmtxLock.lock();
        globals.mpProxyConf->qstrProxyHost = qstrHost;
        globals.mpProxyConf->qstrProxyPort = qstrPort;
        globals.mpProxyConf->qmtxLock.unlock();
    }
    else
        QMessageBox::information(this, "Alert", "Please input proxy host and port!");
}

void DlgProxy::on_btnDisproxy_clicked()
{
    globals.mpProxyConf->qmtxLock.lock();
    globals.mpProxyConf->qstrProxyHost = "";
    globals.mpProxyConf->qstrProxyPort = "";
    globals.mpProxyConf->qmtxLock.unlock();
}
