#include "confirm_interface.h"
#include "ui_confirm_interface.h"
#include <QString>
#include <string>
#include <string.h>
#include <set>
#include <QFont>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

const int MAXN = 1e5 + 5;
const char* SYMBOL = "+-*/()";
const QString DIF[2] = {"      ", "      不"};
const QString DIVISOR[2] = {"\t", "\n"};
const QString MESSAGE[7] = {
    "数量：", "\n格式：", "行 * ", "列\n长度：",
    "等长\n数值：", "\n符号类型：", "\n重复率："};

const QString SMB_TYP[8] = {
    "一则运算 加", "一则运算 减", "一则运算 乘",
    "一则运算 除", "二则运算 加减", "二则运算 乘除",
    "四则运算 带括号", "四则运算 不带括号"};

vector<string> equations;
set<string> tire;

void CreateResult(int row, int col);//将生成算式写入到文件
bool isIllegal(char* sym, int* num, int length, int range);//判断算式合理性
string CreateEquation(int range, int length,
                       int type, bool length_uncertain); //生成单个算式
void CreateEquations(int quantity, double rate, int length,
                     int range, int type, bool length_uncertain); //生成算式集查重并存入

inline QString changeInttoString(int changeInt){ //int转qstring
    return QString::number(changeInt);
}

inline QString changeDoubletoString(double changeDouble){ //double转qstring
    return QString::number(changeDouble);
}

Confirm_Interface::Confirm_Interface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Confirm_Interface)
{
    ui->setupUi(this);

    Message = new QLabel;
    Message->setAlignment(Qt::AlignTop);
    QFont font("Microsoft YaHei", 16);
    Message->setFont(font);

    //确认按钮
    Button_Next = new QPushButton("确认");
    Button_Cancel = new QPushButton("取消");

    //网格布局格式，从左上(0,0)到右下
    QGridLayout *layout = new QGridLayout(this);

    //设置组件位置
    layout-> addWidget(Message, 0, 0, 1, 5);
    layout-> addWidget(Button_Next, 1, 1);
    layout-> addWidget(Button_Cancel, 1, 3);

    connect(Button_Next, SIGNAL(clicked()), this,SLOT(Button_Next_Clicked()));
    connect(Button_Cancel, SIGNAL(clicked()), this,SLOT(Button_Cancel_Clicked()));
}

void Confirm_Interface::Button_Next_Clicked(){//确认要求，开始生成算式
    CreateEquations(quantity, rate, this->length, range, type, length_uncertain);
    CreateResult(row, col);//将生成算式写入到文件

    QApplication* app;
    app->exit(0);
}

void Confirm_Interface::Button_Cancel_Clicked(){
    this->close();
}

Confirm_Interface::~Confirm_Interface()
{
    delete ui;
}

void Confirm_Interface::setMessage(int quantity, int format_row, int format_col, int number_range, int symbol_quantity,
                        bool symbol_quantity_uncertain, int symbol_type, double repetition_rate){

    this->quantity = quantity;
    length = symbol_quantity;
    rate = repetition_rate;
    row = format_row, col = format_col;
    type = symbol_type, range = number_range;
    length_uncertain = symbol_quantity_uncertain;
    QString msg;
    msg += MESSAGE[0] + changeInttoString(quantity);
    msg += MESSAGE[1] + changeInttoString(format_row);
    msg += MESSAGE[2] + changeInttoString(format_col);
    msg += MESSAGE[3] + changeInttoString(symbol_quantity);
    msg += DIF[symbol_quantity_uncertain];
    msg += MESSAGE[4] + changeInttoString(number_range);
    msg += MESSAGE[5] + SMB_TYP[symbol_type];
    msg += MESSAGE[6] + changeDoubletoString(repetition_rate) + "%";
    QByteArray temp_ba = msg.toLatin1();
    Message->setText(temp_ba.data());//显示结果
}

void CreateResult(int row, int col){
    ofstream file("./result.txt", ios::app);

    //写入文件
    for(unsigned int i = 1; i <= equations.size(); i++){
        file << equations[i - 1];
        if(i % col){         //非一行结束，算式后方用空格填充
            for(int j = equations[i - 1].length(); j <= 25; j++)
                file << " ";
        }
        else
            file << "\n";
    }

    //关闭文件
    file.close();
}

void CreateEquations(int quantity, double rate, int length,
                     int range, int type, bool length_uncertain){
    int repetition_limit = rate < 1e5?quantity:1 / rate;//利用重复率粗略计算不能出现重复算式个数
    std::string temp_equation;
    set<std::string>::iterator it;
    while(quantity--){
        temp_equation = CreateEquation(range, length, type, length_uncertain);
        it = tire.find(temp_equation);//使用set查重
        while(it != tire.end()){
            temp_equation = CreateEquation(range, length, type, length_uncertain);
            it = tire.find(temp_equation);
        }
        int sz = equations.size();
        if(sz >= repetition_limit)//控制查重算式个数，控制在最近的1/rate之内
            tire.erase(equations[sz - repetition_limit]);
        tire.insert(temp_equation);//插入查重集
        equations.push_back(temp_equation);//插入结果集
    }
}

string CreateEquation(int range, int length,
                       int type, bool length_uncertain){
    //srand(time(0));

    int num[5] = {0};
    char sym[6] = {0};
    int lengthadd = 0;
    int limit_Minn, type_num; //符号限制
    if(type < 4){//符号种类与在SYMBOL中开始位置赋值
        limit_Minn = type;
        type_num = 1;
    }
    else if(type < 6){
        limit_Minn = type == 4?0:2;
        type_num = 2;
    }
    else{
        if(type == 6 && rand() % 2)//随机是否带括号
            lengthadd = 2;
        limit_Minn = 0;
        type_num = 4;
    }
    if(length_uncertain)//如果算式长度可以不等长，随机值
        length = rand() % length + 1;
    if(length < 2)//算式长度小于2，不必加括号
        lengthadd = 0;
    if(lengthadd){//随机出括号位置
        int pos = rand() % length;
        sym[pos] = '(';
        sym[pos + 2] = ')';//设定括号内只能存在一个运算符
    }

    for(int i = 0; i < length + lengthadd; i++){//随机其他位置的运算符
        if(sym[i] == 0)
            sym[i] = SYMBOL[rand() % type_num + limit_Minn];
    }
    for(int i = 0; i <= length; i++){//随机数字
        num[i] = rand() % range;
        while(rand() % 100 < 90 && !num[i]) //尽量避免0出现
            num[i] = rand() % range;
    }

    while(!isIllegal(sym, num, length, range)){//判断合法性，非法，再随机生成一次

        for(int i = 0; i < length + lengthadd; i++){//随机其他位置的运算符
            if(sym[i] == 0)
                sym[i] = rand() % type_num + limit_Minn;
        }
        for(int i = 0; i <= length; i++){//随机数字
            num[i] = rand() % range;
            while(rand() % 100 < 90 && !num[i]) //尽量避免0出现
                num[i] = rand() % range;
        }
    }
    string equation = "";//组成算式，格式控制
    QString temp_qstr;
    int sym_pos = 0;
    if(sym[0] == '('){//第一个符号是括号的情况
        equation += '(';
        sym_pos++;
    }
    temp_qstr = changeInttoString(num[0]);//算式中加上数字
    equation += temp_qstr.toStdString() + " ";//算式中加上数字后与运算符保持间隔
    for(int num_pos = 1; num_pos <= length; num_pos++){
        equation += sym[sym_pos++];//算式中加上运算符
        equation += " ";//算式中加上运算符后与数字保持间隔
        if(sym[sym_pos] == '('){
            equation += '(';
            sym_pos++;
        }
        temp_qstr = changeInttoString(num[num_pos]);
        equation += temp_qstr.toStdString();//算式中加上数字
        if(sym[sym_pos] == ')'){
            equation += ')';
            sym_pos++;
        }
        equation += " ";//算式中加上数字及括号后与运算符保持间隔
    }
    equation += "=";
    return equation;
}

bool isIllegal(char* sym, int* num, int length, int range){
    int num_stack[5];
    char sym_stack[6];
    int num_top = 0, sym_top = -1;
    int num_pos = 0, sym_pos = 0;
    num_stack[num_top] = num[num_pos];
    while(++num_pos <= length){//数字及运算符入栈
        if(sym[sym_pos] == '('){//括号情况下优先级最高，只考虑仅有一个括号及括号内仅有一个运算符情况
            sym_pos++;
            if(sym[sym_pos] == '*')
                num_stack[num_top] *= num[num_pos];
            else if(sym[sym_pos] == '/'){
                if(!num[num_pos] || num_stack[num_top] % num[num_pos])
                    return false;
                num_stack[num_top] /= num[num_pos];
            }
            if(sym[sym_pos] == '+')
                num_stack[num_top] += num[num_pos];
            if(sym[sym_pos] == '-')
                num_stack[num_top] -= num[num_pos];
            if(num_stack[num_top] < 0 || num_stack[num_top] > range)
                return false;
            sym_pos += 2;
        }
        else if(sym[sym_pos] == '*' || sym[sym_pos] == '/'){//*/入栈前要清出栈顶的*/
            while(~sym_top && (sym_stack[sym_top] == '*' || sym_stack[sym_top] == '/')){
                if(sym_stack[sym_top] == '*')
                    num_stack[num_top - 1] *= num_stack[num_top];
                else if(sym_stack[sym_pos] == '/'){
                    if(!num_stack[num_top] || num_stack[num_top - 1] % num_stack[num_top])
                        return false;
                    num_stack[num_top - 1] /= num_stack[num_top];
                }
                num_top--, sym_top--;
                if(num_stack[num_top] < 0 || num_stack[num_top] > range)
                    return false;
            }
            num_stack[++num_top] = num[num_pos];
            sym_stack[++sym_top] = sym[sym_pos++];
        }
        else if(sym[sym_pos] == '+' || sym[sym_pos] == '-'){//+-入栈前要清空栈
            while(~sym_top){
                if(sym_stack[sym_top] == '*')
                    num_stack[num_top - 1] *= num_stack[num_top];
                else if(sym_stack[sym_top] == '/'){
                    if(!num_stack[num_top] || num_stack[num_top - 1] % num_stack[num_top])
                        return false;
                    num_stack[num_top - 1] /= num_stack[num_top];
                }
                else if(sym_stack[sym_top] == '+')
                    num_stack[num_top - 1] += num_stack[num_top];
                else if(sym_stack[sym_top] == '-')
                    num_stack[num_top - 1] -= num_stack[num_top];
                num_top--, sym_top--;
                if(num_stack[num_top] < 0 || num_stack[num_top] > range)
                    return false;
            }
            num_stack[++num_top] = num[num_pos];
            sym_stack[++sym_top] = sym[sym_pos++];
        }
        else
            return false;
    }
    while(~sym_top){//清栈
        if(sym_stack[sym_top] == '*')
            num_stack[num_top - 1] *= num_stack[num_top];
        else if(sym_stack[sym_top] == '/'){
            if(!num_stack[num_top] || num_stack[num_top - 1] % num_stack[num_top])
                return false;
            num_stack[num_top - 1] /= num_stack[num_top];
        }
        else if(sym_stack[sym_top] == '+')
            num_stack[num_top - 1] += num_stack[num_top];
        else if(sym_stack[sym_top] == '-')
            num_stack[num_top - 1] -= num_stack[num_top];
        num_top--, sym_top--;
        if(num_stack[num_top] < 0 || num_stack[num_top] > range)
            return false;
    }
    return true;
}
