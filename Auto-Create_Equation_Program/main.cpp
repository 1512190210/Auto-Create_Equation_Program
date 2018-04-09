#include "query_interface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF8"));
    QApplication a(argc, argv);
    Query_Interface w;
    w.setWindowTitle(QObject::tr("自动生成算式器"));
    w.show();
    
    return a.exec();
}
