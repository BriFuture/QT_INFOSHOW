#ifndef UTILS_FILEUTIL_H
#define UTILS_FILEUTIL_H

#include <QString>

namespace utils {
class FileUtil;
}

class FileUtil {
public:
    static QString read(QString filename);
    static bool write(QString filename, QString content);
    static QString readAll(QString filename);
};

#endif // UTILS_FILEUTIL_H
