#ifndef COMPOSER_H
#define COMPOSER_H

#include <QWidget>
#include <QThread>

namespace Ui {
class composer;
}

class composer : public QWidget
{
    Q_OBJECT
    
public:
    explicit composer(QWidget *parent = 0);
    ~composer();
    //data
    QString *mqstrRequ;
    
protected:
    void customEvent(QEvent *event);

private slots:
    void on_btnGenerate_clicked();

    void on_btnSend_clicked();

private:
    Ui::composer *ui;
};

//communicating thread
class CommThrd: public QThread
{
    Q_OBJECT

public:
    explicit CommThrd(QString *&pqstrRequ)
    {
        mpqstrRequ = pqstrRequ ;
    }

    //data
    QString *mpqstrRequ;

protected:
    void run();
};

#endif // COMPOSER_H
