#include "query_interface.h"
#include "ui_query_interface.h"

#define QUANTITY_LOCK 0 //数量
#define ROW_LOCK 1      //行
#define COL_LOCK 2      //列
#define RANGE_LOCK 3    //数值
#define RATE_LOCK 4     //重复率
#define ILLEGAL_LOCK 5  //合理性

const char *SYMBOL_TYPE[8] = {
    "一则运算 加", "一则运算 减", "一则运算 乘",
    "一则运算 除", "二则运算 加减", "二则运算 乘除",
    "四则运算 带括号", "四则运算 不带括号"};

Query_Interface::Query_Interface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Query_Interface){
    //初始化
    ui-> setupUi(this);

    quantity = format_row = number_range = 10;
    format_col = symbol_quantity = 1;
    symbol_quantity_uncertain = false;
    firsttime_click_next = false;
    symbol_type = repetition_rate = 0;
    lock = 0; //禁用确认按钮锁

    QWidget *widget = new QWidget;
    this-> setCentralWidget(widget);
    this-> setFixedSize(this->width(), this->height());//禁止放大

    //实例化并加入提示
    //算式数量
    Quantity_Head_Tip = new QLabel(tr("数量："));
    Quantity_Edit = new QLineEdit(tr("10"));
    Quantity_Wranning = new QLabel;
    Quantity_Wranning->setStyleSheet("color:red;");

    //格式
    Format_Head_Tip = new QLabel(tr("格式："));
    Format_Row_Edit = new QLineEdit(tr("10"));//行
    Format_Mid_Tip = new QLabel(tr("行 * "));
    Format_Col_Edit = new QLineEdit(tr("1"));//列
    Format_Rear_Tip = new QLabel(tr("列"));
    Format_Wranning = new QLabel;
    Format_Wranning->setStyleSheet("color:red;");

    //数值范围
    Number_Range_Head_Tip = new QLabel(tr("数值"));
    Number_Range_Edit = new QLineEdit(tr("10"));
    Number_Range_Wranning = new QLabel;
    Number_Range_Wranning->setStyleSheet("color:red;");

    //符号数量
    Symbol_Quantity_Head_Tip = new QLabel(tr("长度："));
    Symbol_Quantity_Edit = new QComboBox;
    Symbol_Quantity_Edit-> addItem(tr("1"));
    Symbol_Quantity_Edit-> addItem(tr("2"));
    Symbol_Quantity_Edit-> addItem(tr("3"));
    Symbol_Quantity_Uncertain = new QCheckBox;//算式不等长
    Symbol_Quantity_Rear_Tip = new QLabel(tr("不等长"));

    //符号类型
    Symbol_Type_Head_Tip = new QLabel(tr("类型："));
    Symbol_Type_Edit = new QComboBox;
    for(int pos = 0; pos < 8; pos++)
        Symbol_Type_Edit-> addItem(tr(SYMBOL_TYPE[pos]));

    //重复率
    Repetition_Rate_Head_Tip = new QLabel(tr("重复率："));
    Repetition_Rate_Edit = new QLineEdit(tr("0"));
    Repetition_Rate_Rear_Tip = new QLabel(tr("%"));
    Repetition_Rate_Wranning = new QLabel;//隐藏提示，要求输入合法性
    Repetition_Rate_Wranning->setStyleSheet("color:red;");

    Illegal_Wranning = new QLabel;//隐藏提示，要求输入合法性
    Illegal_Wranning->setStyleSheet("color:red;");

    //确认按钮
    Button_Next = new QPushButton("确认");
    Button_Cancel = new QPushButton("取消");

    //网格布局格式，从左上(0,0)到右下
    QGridLayout *layout = new QGridLayout(this);

    //设置组件位置
    //数量
    layout-> addWidget(Quantity_Head_Tip, 0, 0);
    layout-> addWidget(Quantity_Edit, 0, 1);
    layout-> addWidget(Quantity_Wranning, 0, 2, 1, 5);

    //算式格式
    layout-> addWidget(Format_Head_Tip, 1, 0);
    layout-> addWidget(Format_Row_Edit, 1, 1);//行
    layout-> addWidget(Format_Mid_Tip, 1, 2, Qt::AlignHCenter);
    layout-> addWidget(Format_Col_Edit, 1, 3);//列
    layout-> addWidget(Format_Rear_Tip, 1, 4);
    layout-> addWidget(Format_Wranning, 1, 5);

    //数值范围
    layout-> addWidget(Number_Range_Head_Tip, 2, 0);
    layout-> addWidget(Number_Range_Edit, 2, 1);
    layout-> addWidget(Number_Range_Wranning, 2, 2, 1, 5);

    //符号数量
    layout-> addWidget(Symbol_Quantity_Head_Tip, 3, 0);
    layout-> addWidget(Symbol_Quantity_Edit, 3, 1, 1, 1);
    layout-> addWidget(Symbol_Quantity_Uncertain, 3, 2, Qt::AlignRight);//算式不等长
    layout-> addWidget(Symbol_Quantity_Rear_Tip, 3, 3);

    //符号类型
    layout-> addWidget(Symbol_Type_Head_Tip, 4, 0);
    layout-> addWidget(Symbol_Type_Edit, 4, 1);

    //重复率
    layout-> addWidget(Repetition_Rate_Head_Tip, 5, 0);
    layout-> addWidget(Repetition_Rate_Edit, 5, 1);
    layout-> addWidget(Repetition_Rate_Rear_Tip, 5, 2);
    layout-> addWidget(Repetition_Rate_Wranning, 5, 3, 1, 5);

    layout-> addWidget(Illegal_Wranning, 6, 1, 1, 5);

    //确认按钮
    layout-> addWidget(Button_Next, 7, 1);
    layout-> addWidget(Button_Cancel, 7, 3);

    widget-> setLayout(layout); //将布局显示出来

    //事件触发条件设置，与相应槽相连
    connect(Quantity_Edit, SIGNAL(textChanged(const QString &)), this,SLOT(Lineedit_Quantity(const QString &)));
    connect(Format_Row_Edit, SIGNAL(textChanged(const QString &)), this,SLOT(Lineedit_Format_Row(const QString &)));
    connect(Format_Col_Edit, SIGNAL(textChanged(const QString &)), this,SLOT(Lineedit_Format_Col(const QString &)));
    connect(Number_Range_Edit, SIGNAL(textChanged(const QString &)), this,SLOT(Lineedit_Number_Range(const QString &)));
    connect(Symbol_Quantity_Edit, SIGNAL(currentIndexChanged(const QString &)), this,SLOT(Combobox_Symbol_Quantity(const QString &)));
    connect(Symbol_Quantity_Uncertain, SIGNAL(clicked()), this,SLOT(Checkbox_Symbol_Quantity_Uncertain()));
    connect(Symbol_Type_Edit, SIGNAL(currentIndexChanged(const QString &)), this,SLOT(Combobox_Symbol_Type(const QString &)));
    connect(Repetition_Rate_Edit, SIGNAL(textChanged(const QString &)), this,SLOT(Lineedit_Repetition_Rate(const QString &)));
    connect(Button_Next, SIGNAL(clicked()), this,SLOT(Button_Next_Clicked()));
    connect(Button_Cancel, SIGNAL(clicked()), this,SLOT(Button_Cancel_Clicked()));
}

void Query_Interface::Lineedit_Quantity(const QString &quantity){
    bool ok;
    int Quantity = quantity.toInt(&ok, 10);
    if(!ok || Quantity <= 0 || Quantity > 100000){
        Quantity_Wranning-> setText("请输入100000以内的正整数！");
        lock |= 1 << QUANTITY_LOCK; //加锁
        Button_Next->setEnabled(false);
    }
    else{
        this-> quantity = format_row = Quantity;
        format_col = 1;
        isIllegal();
        Format_Row_Edit-> setText(QString::number(Quantity, 10));
        Format_Col_Edit-> setText("1");
        Quantity_Wranning-> setText("");
        Format_Wranning-> setText("");
        lock ^= 1 << QUANTITY_LOCK; //解锁
        lock ^= 1 << ROW_LOCK;
        lock ^= 1 << COL_LOCK;
        lock = !lock;

        if(!lock)
            Button_Next->setEnabled(true);
    }
}

void Query_Interface::Lineedit_Format_Row(const QString &row){
    bool ok;
    int Row = row.toInt(&ok, 10);
    if(ok && Row > 0 && quantity % Row == 0){
        format_row = Row;
        format_col = quantity / Row;
        Format_Col_Edit-> setText(QString::number(format_col, 10));
        Format_Wranning-> setText("");
        lock ^= 1 << ROW_LOCK; //解锁
        lock = !lock;

        if(!lock)
            Button_Next->setEnabled(true);
    }
    else{
        Format_Wranning-> setText("行/列应整除数量！");
        lock |= 1 << ROW_LOCK; //加锁
        Button_Next->setEnabled(false);
    }
}

void Query_Interface::Lineedit_Format_Col(const QString &col){
    bool ok;
    int Col = col.toInt(&ok, 10);
    if(ok && Col > 0 && quantity % Col == 0){
        format_col = Col;
        format_row = quantity / Col;
        Format_Row_Edit-> setText(QString::number(format_row, 10));
        Format_Wranning-> setText("");
        lock ^= 1 << COL_LOCK; //解锁
        lock = !lock;

        if(!lock)
            Button_Next->setEnabled(true);
    }
    else{
        Format_Wranning->setText("行/列应整除数量！");
        lock |= 1 << COL_LOCK; //加锁
        Button_Next->setEnabled(false);
    }
}

void Query_Interface::Lineedit_Number_Range(const QString &range){
    bool ok;
    int Range = range.toInt(&ok, 10);
    if(!ok || Range <= 0 || Range > 10000){
        Number_Range_Wranning-> setText("请输入10000以内的正整数！");
        lock |= 1 << RANGE_LOCK; //加锁
        Button_Next->setEnabled(false);
    }
    else{
        number_range = Range;
        Number_Range_Edit-> setText(QString::number(Range, 10));
        Number_Range_Wranning-> setText("");
        isIllegal();
        lock ^= 1 << RANGE_LOCK; //解锁
        lock = !lock;
        if(!lock)
            Button_Next->setEnabled(true);
    }
}

void Query_Interface::Combobox_Symbol_Quantity(const QString &quantity){
    symbol_quantity = quantity.toInt();
    isIllegal();
}

void Query_Interface::Checkbox_Symbol_Quantity_Uncertain(){
    symbol_quantity_uncertain = !symbol_quantity_uncertain;
}

void Query_Interface::Combobox_Symbol_Type(const QString &type){
    std::string temp_string = type.toStdString();
    const char* type_char = temp_string.c_str();

    for(int pos = 0; pos < 8; pos++)
        if(!strcmp(SYMBOL_TYPE[pos], type_char))
            symbol_type = pos;
    isIllegal();
}

void Query_Interface::Lineedit_Repetition_Rate(const QString &rate){
    bool ok;
    double Rate = rate.toDouble(&ok);
    if(!ok || Rate < 0 || Rate > 100){
        Repetition_Rate_Wranning-> setText("百分比不得超过100！");
        lock |= 1 << RATE_LOCK;
        Button_Next->setEnabled(false);
    }
    else{
        repetition_rate = Rate;
        isIllegal();
        Repetition_Rate_Wranning-> setText("");
        lock ^= 1 << RATE_LOCK;
        lock = !lock;

        if(!lock)
            Button_Next->setEnabled(true);
    }
}

void Query_Interface::Button_Next_Clicked(){
    this->visibleRegion();
    if(!firsttime_click_next){
        view = new Confirm_Interface(this);//将类指针实例化
        firsttime_click_next = true;
    }
    view->setMessage(quantity, format_row, format_col, number_range, symbol_quantity,
                     symbol_quantity_uncertain, symbol_type, repetition_rate);
    view->show();
}

void Query_Interface::Button_Cancel_Clicked(){

    QApplication* app;
    app->exit(0);
}

//要求合理性
bool Query_Interface::isIllegal(){
    if(symbol_quantity == 1 && symbol_type == 6){//算式长度与符号类型的矛盾
        lock |= 1 << ILLEGAL_LOCK; //加锁
        Illegal_Wranning-> setText("请检查数量、数值、重复率、符号类型等！");
        Button_Next->setEnabled(false);
        return false;
    }
    long long number = number_range;
    for(int i = 0; i < symbol_quantity; i++)
        number *= number_range;
    number /= (1 - repetition_rate / 100);
    if(number < quantity){//要求计算结果不合理
        lock |= 1 << ILLEGAL_LOCK; //加锁
        Illegal_Wranning-> setText("请检查数量、数值、重复率、符号类型等！");
        Button_Next->setEnabled(false);
        return false;
    }
    Illegal_Wranning-> setText("");
    lock ^= 1 << ILLEGAL_LOCK; //解锁
    lock = !lock;
    if(!lock)
        Button_Next->setEnabled(true);
    return true;
}

Query_Interface::~Query_Interface(){
    delete ui;
}
