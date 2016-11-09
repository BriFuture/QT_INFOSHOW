#ifndef UTILS_FILEUTIL_H
#define UTILS_FILEUTIL_H

#include <QString>

namespace utils {
class FileUtil;
}

class FileUtil {
private:
    static QString appCurrentPath;
public:
    const static QString NEW_LINE;
public:
    static QString read(QString filename);
    static bool write(QString filename, QString content);
    static QString readAll(QString filename);
    static QString getAppCurrentPath();
    static void setAppCurrentPath(QString path);
};

#endif // UTILS_FILEUTIL_H
