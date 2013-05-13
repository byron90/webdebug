#include "wdebug.h"
#include "ui_wdebug.h"

wdebug::wdebug(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wdebug)
{
    // show ui
    ui->setupUi(this);
    pwinMonitor = new monitor(ui->frame_2);
    ui->horizontalLayout_3->addWidget(pwinMonitor);
        //pSubwin->hide();
    //server
    mpServer = new WDServer() ;
    mpServer->mBeginListen() ;
}

wdebug::~wdebug()
{
    delete ui;
}

void wdebug::on_pushButton_2_clicked()
{
    pwinMonitor->show();
}
