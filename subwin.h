#ifndef SUBWIN_H
#define SUBWIN_H

#include <QWidget>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDataStream>
#include <QMessageBox>
#include <QFile>

namespace Ui {
class Subwin;
}

class Subwin : public QWidget
{
    Q_OBJECT
    
public:
    explicit Subwin(QWidget *parent = 0);
    ~Subwin();
    
private slots:
    void on_btnBeginLisn_clicked();
    void mthListen();
    void on_btnEndLisn_clicked();

private:
    Ui::Subwin *ui;
    QTcpServer * pmTcpServer;
    QTcpSocket * pmTcpSocket;
};

#endif // SUBWIN_H
