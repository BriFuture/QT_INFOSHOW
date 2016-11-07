#include "FileUtil.h"
#include <QFile>
#include <QTextStream>

/** 读取文本内容 **/
QString FileUtil::read(QString filename) {
    QFile qf(filename);
    if(!qf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }
    QTextStream its(&qf);
    QString str = its.readLine();
    QString result;

    while(!str.isNull()) {
        // 添加一个换行符
        result += str + '\n';
        str = its.readLine();
    }

    qf.close();
    // 去掉最后加上的换行符
    return result.trimmed();
}

QString FileUtil::readAll(QString filename) {
    QFile qf(filename);
    if(!qf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }
    QTextStream its(&qf);
    QString result = its.readAll();
    qf.close();
    // 去掉最后加上的换行符
    return result.trimmed();
}

/** 写入内容 **/
bool FileUtil::write(QString filename, QString content) {
    QFile qf(filename);
    if(!qf.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream ots(&qf);
    ots << content;
    qf.close();
    return true;
}
