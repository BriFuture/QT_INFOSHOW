#include "ui/mainwindow.h"
#include "ui/FloatWidget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    // 设置字符编码为 UTF-8
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
//    MainWindow w;
//    w.show();
    FloatWidget fw;
    fw.show();


    return a.exec();
}
