#ifndef CONFIRM_INTERFACE_H
#define CONFIRM_INTERFACE_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

namespace Ui {
class Confirm_Interface;
}

class Confirm_Interface : public QDialog
{
    Q_OBJECT

public:
    explicit Confirm_Interface(QWidget *parent = 0);
    ~Confirm_Interface();

    void setMessage(int, int, int, int, int, bool, int, double);

private:
    Ui::Confirm_Interface *ui;

    int quantity;
    int length, type;
    bool length_uncertain;
    int range;
    int row, col;
    double rate;


    QLabel *Message;

    //确认取消按钮
    QPushButton *Button_Next;
    QPushButton *Button_Cancel;

//槽
private slots:

    void Button_Next_Clicked();
    void Button_Cancel_Clicked();
};

#endif // CONFIRM_INTERFACE_H
