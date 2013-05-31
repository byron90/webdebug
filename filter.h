#ifndef FILTER_H
#define FILTER_H

#include <QWidget>
#include "wdglobal.h"

namespace Ui {
class filter;
}

class filter : public QWidget
{
    Q_OBJECT
    
public:
    explicit filter(QWidget *parent = 0);
    ~filter();
    
private slots:
    void on_btnAddBkUrl_clicked();

    void on_btnBkUrlDetl_clicked();

private:
    Ui::filter *ui;
};

#endif // FILTER_H
