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

    void on_btnAutoPath_clicked();

    void on_btnAddAutoResp_clicked();

    void on_btnAutoDetl_clicked();

    void on_cboxHide2_stateChanged(int arg1);

    void on_cboxHide304_stateChanged(int arg1);

    void on_cboxHide302_stateChanged(int arg1);

    void on_cboxHiden2_stateChanged(int arg1);

    void on_cboxScript_stateChanged(int arg1);

    void on_cboxImg_stateChanged(int arg1);

    void on_cboxCSS_stateChanged(int arg1);

    void on_cboxSwf_stateChanged(int arg1);

private:
    //specific src file of auto response
    QString mqstrSrcPath;
    Ui::filter *ui;
};

#endif // FILTER_H
