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

    //set icon
    setWindowIcon(QIcon("wd.ico"));

    //create fancytabwidget
    mpFancyTab = new FancyTabWidget(this) ;
    setCentralWidget(mpFancyTab);
    //***create child widgets
    pwinMonitor = new monitor(mpFancyTab);
    pwinComposer = new composer(mpFancyTab);
    pwinFilter = new filter(mpFancyTab);
    pwinBreakPnt = new breakpoint(mpFancyTab);
    //***add to fancytabwidget
    mpFancyTab->insertTab(0, pwinMonitor, QIcon("img/monitor.png"),"Monitor");
    mpFancyTab->setTabEnabled(0, true);
    mpFancyTab->insertTab(1, pwinComposer,QIcon("img/composer.png"),"Composer" );
    mpFancyTab->setTabEnabled(1, true);
    mpFancyTab->insertTab(2, pwinFilter,QIcon("img/filter.png"),"Filter" );
    mpFancyTab->setTabEnabled(2, true);
    mpFancyTab->insertTab(3, pwinBreakPnt,QIcon("img/breakpoint.png"),"Breakpoint" );
    mpFancyTab->setTabEnabled(3, true);
    setDockNestingEnabled(true);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
    //bind actions
    //*** agent switch
    connect(ui->actAgent, SIGNAL(triggered()), this, SLOT(switchAgent()));
    //*** exit
    connect(ui->actExit, SIGNAL(triggered()), this, SLOT(exitProg()));
    //*** tabs
    mpsMapper = new QSignalMapper(this);
    connect(ui->actMonitor, SIGNAL(triggered()), mpsMapper, SLOT(map()));
    mpsMapper->setMapping(ui->actMonitor, 0);
    connect(ui->actComposer, SIGNAL(triggered()), mpsMapper, SLOT(map()));
    mpsMapper->setMapping(ui->actComposer, 1);
    connect(ui->actFilter, SIGNAL(triggered()), mpsMapper, SLOT(map()));
    mpsMapper->setMapping(ui->actFilter, 2);
    connect(ui->actBreakp, SIGNAL(triggered()), mpsMapper, SLOT(map()));
    mpsMapper->setMapping(ui->actBreakp, 3);
    connect(mpsMapper, SIGNAL(mapped(int)), this, SLOT(switchTabs(int)));

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

//switch agent on or off
void wdebug::switchAgent()
{
    if(ui->actAgent->text()=="Agent on")
    {//start the agent
        if(!mpServer->mStartAgent())
        {//alert that the agent can't start
            QMessageBox::warning(this, "Warning", "Can't start the agent!") ;
            return ;
        }
        ui->actAgent->setText("Agent off");
    }
    else
    {//pause the agent
        if(!mpServer->mPauseAgent())
        {
            QMessageBox::warning(this, "Warning", "Can't start the agent!") ;
            return ;
        }
        ui->actAgent->setText("Agent on");
    }
}

//switch tabs
void wdebug::switchTabs(int iTabIndex)
{
    mpFancyTab->setCurrentIndex(iTabIndex);
}

//exit the program
void wdebug::exitProg()
{
    qApp->exit(0);
}
