#ifndef QUERY_INTERFACE_H
#define QUERY_INTERFACE_H

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QTextCodec>
#include <QtCore/qmath.h>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QStack>
#include "Confirm_Interface.h"

//开辟命名空间
namespace Ui {
class Query_Interface;
}

class Query_Interface : public QMainWindow
{
    Q_OBJECT

public:
    explicit Query_Interface(QWidget *parent = 0);//Widge
    ~Query_Interface();
private:
    Ui::Query_Interface *ui;

    int lock;
    int quantity;
    int format_row, format_col;
    int number_range;
    int symbol_quantity;
    bool symbol_quantity_uncertain;
    bool firsttime_click_next;
    int symbol_type;
    double repetition_rate;

    Confirm_Interface *view;//创建新窗口类的指针

    //算式数量
    QLabel *Quantity_Head_Tip;
    QLineEdit *Quantity_Edit;
    QLabel *Quantity_Wranning;//隐藏提示，计算要求合法性

    //算式格式
    QLabel *Format_Head_Tip;
    QLineEdit *Format_Row_Edit;//行
    QLabel *Format_Mid_Tip;
    QLineEdit *Format_Col_Edit;//列
    QLabel *Format_Rear_Tip;
    QLabel *Format_Wranning;//隐藏提示，要求输入合法性

    //数值范围
    QLabel *Number_Range_Head_Tip;
    QLineEdit *Number_Range_Edit;
    QLabel *Number_Range_Wranning;//隐藏提示，要求输入合法性

    //符号数量
    QLabel *Symbol_Quantity_Head_Tip;
    QComboBox *Symbol_Quantity_Edit;
    QCheckBox *Symbol_Quantity_Uncertain;//算式不等长
    QLabel *Symbol_Quantity_Rear_Tip;

    //符号类型
    QLabel *Symbol_Type_Head_Tip;
    QComboBox *Symbol_Type_Edit;

    //重复率
    QLabel *Repetition_Rate_Head_Tip;
    QLineEdit *Repetition_Rate_Edit;
    QLabel *Repetition_Rate_Rear_Tip;
    QLabel *Repetition_Rate_Wranning;//隐藏提示，要求输入合法性

    QLabel *Illegal_Wranning;//隐藏提示，要求输入合法性

    //确认取消按钮
    QPushButton *Button_Next;
    QPushButton *Button_Cancel;

    bool isIllegal();  //计算要求合理性
//槽
private slots:

    void Lineedit_Quantity(const QString &);
    void Lineedit_Format_Row(const QString &);
    void Lineedit_Format_Col(const QString &);
    void Lineedit_Number_Range(const QString &);
    void Combobox_Symbol_Quantity(const QString &);
    void Checkbox_Symbol_Quantity_Uncertain();
    void Combobox_Symbol_Type(const QString &);
    void Lineedit_Repetition_Rate(const QString &);
    void Button_Next_Clicked();
    void Button_Cancel_Clicked();
};

#endif // QUERY_INTERFACE_H
