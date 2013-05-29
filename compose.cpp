#include "compose.h"
#include "ui_compose.h"

compose::compose(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::compose)
{
    ui->setupUi(this);
}

compose::~compose()
{
    delete ui;
}
