#include "ui/mainwindow.h"
#include "ui/FloatWidget.h"
#include "main/Configure.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    // 设置字符编码为 UTF-8
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    FloatWidget *fw = new FloatWidget;
    fw->show();
    return a.exec();
}
