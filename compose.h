#ifndef COMPOSE_H
#define COMPOSE_H

#include <QWidget>

namespace Ui {
class compose;
}

class compose : public QWidget
{
    Q_OBJECT
    
public:
    explicit compose(QWidget *parent = 0);
    ~compose();
    
private:
    Ui::compose *ui;
};

#endif // COMPOSE_H
