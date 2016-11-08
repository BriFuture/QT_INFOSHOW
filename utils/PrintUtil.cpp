#include "PrintUtil.h"
#include "version.h"
#include <QDebug>
#include <execinfo.h>
#include <cxxabi.h>

/** 提供接口，可供调试或仅输出 **/
void PrintUtil::print(QString str, QString flag) {
    /* 如果全局设定可以输出调试信息，则调用 debug */
    if(revision_beta) {
        debug(str, flag);
    } else {
        out(str);
    }
}

void PrintUtil::print(int num, QString flag) {
    print(QString::number(num), flag);
}

void PrintUtil::print(float fnum, QString flag) {
    print(QString::number(fnum), flag);
}

void PrintUtil::print(double dnum, QString flag) {
    print(QString::number(dnum), flag);
}

// 直接输出调试信息
void PrintUtil::debug(QString str, QString flag) {
    printSeperator(flag, 1);  // 分隔符 start
    out(str);
    printSeperator(flag, 2);  // 分隔符 end
}
/** 输出信息 */
void PrintUtil::out(QString str) {
    qDebug() << str;
}
// 打印分隔符
void PrintUtil::printSeperator(QString flag, int soe, int seperator_num) {
    /* 如果 flag 不为空，打印分隔符 */
    if(!flag.isEmpty()) {
        QString str;
        str.fill('=', seperator_num);
        QString dstr;
        if(soe == 1) {
            dstr = " start of ";
        } else if(soe == 2) {
            dstr = "  end  of ";
        }
        flag = str + dstr + flag + " " + str;
        out(flag);
    }
}
