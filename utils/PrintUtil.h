#ifndef UTILS_PRINTUTIL_H
#define UTILS_PRINTUTIL_H

#include <QString>

namespace utils {
class PrintUtil;
}

class PrintUtil {
private:
    const static int STACK_SIZE = 4;
private:
    static void out(QString);

public:
    const static int SEPERERATOR_NUM = 15;
public:
    /**
     * 打印分割符
     * @param flag 为分隔符中间的标识，用于区分不同分隔符
     * @param soe   当 soe == 1 时，在 flag 前面加上 "start of"
     *              当 soe == 2 时，在 flag 前面加上 "end of"
     *              当 soe == 0 时，不添加字符串
     *              默认为 0
     * @param seperator_num  打印分隔符 "=" 的次数，默认为 SEPERATOR_NUM
    **/
    static void printSeperator(QString flag, int soe=0, int seperator_num=SEPERERATOR_NUM);
    /**
     * 打印调试信息
     * @param str 需要输出的调试信息
     * @param flag 作为分隔符的界定符
    **/
    static void debug(QString str, QString flag);
    /**
     * 输出信息或调试信息
     * @param str  需要输出的信息
     * @param flag  分隔符的界定符，默认为空字符串
    **/
    static void print(QString str, QString flag="");
};

#endif // TIPUTIL_H
