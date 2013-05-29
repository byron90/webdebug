#include <QMessageBox>
#include "wdebug.h"
#include "ui_wdebug.h"
#include "utils/manhattanstyle.h"

wdebug::wdebug(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wdebug)
{
    using namespace Core ;
    using namespace Core::Internal ;

    // show ui
    ui->setupUi(this);
    qApp->setStyle(new ManhattanStyle(QApplication::style()->objectName())) ;

    //create fancytabwidget
    mpFancyTab = new FancyTabWidget(this) ;
    setCentralWidget(mpFancyTab);
    //child widget
    pwinMonitor = new monitor(mpFancyTab);
    mpFancyTab->insertTab(0, pwinMonitor, QIcon("img/monitor.png"),"Monitor");
    mpFancyTab->setTabEnabled(0, true);
    mpFancyTab->insertTab(1, new QWidget(this),QIcon("./images/pause.png"),"Run" );
    mpFancyTab->setTabEnabled(1, true);
    setDockNestingEnabled(true);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);

//    ui->horizontalLayout_chrd->addWidget(pwinMonitor);
}

wdebug::~wdebug()
{
    delete ui;
}

//create and start the server to monitor
bool wdebug::mStartServer()
{
    mpServer = new WDServer() ;
    if(mpServer->mStartAgent())
        return true ;
    else
        return false ;
}

//void wdebug::on_btnSwitch_clicked()
//{
//    if(ui->btnSwitch->text()=="start")
//    {//start the agent
//        if(!mpServer->mStartAgent())
//        {//alert that the agent can't start
//            QMessageBox::warning(this, "Warning", "Can't start the agent!") ;
//            return ;
//        }
//        ui->btnSwitch->setText("pause");
//    }
//    else
//    {//pause the agent
//        if(!mpServer->mPauseAgent())
//        {
//            QMessageBox::warning(this, "Warning", "Can't start the agent!") ;
//            return ;
//        }
//        ui->btnSwitch->setText("start");
//    }
//}
