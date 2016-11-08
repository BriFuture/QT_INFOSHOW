#include "Configure.h"
#include "../utils/FileUtil.h"
#include "../utils/PrintUtil.h"
#include <QMapIterator>

const QString Configure::CONFIGURE_FILE = "./is.config";
const QString Configure::CONFIGURE_SEPE_FLAG = " :: ";
Configure * Configure::config = NULL;

Configure::Configure() {

}

Configure::~Configure() {
    delete config;
}

QStringList Configure::getAllKeys() {
    QStringList sl(cmap.keys());
    return sl;
}

Configure * Configure::getConfigure() {
    if(config == NULL) {
        config = new Configure;
    }
    return config;
}

QVariant Configure::getValue(QString key, QVariant v) {
    return cmap.value(key, v);
}

QString Configure::toString() {
    QMapIterator<QString, QVariant> mit(cmap);
//    PrintUtil::print(cmap.size(), "SIZE");
    QString str;
    while(mit.hasNext()) {
        mit.next();
//        PrintUtil::printSeperator("mit");
//        PrintUtil::print(mit.key());
//        PrintUtil::print(mit.value().toString());
//        PrintUtil::printSeperator("mit");
        str.append(mit.key() + CONFIGURE_SEPE_FLAG + mit.value().toString() + FileUtil::NEW_LINE);
    }
    PrintUtil::print(str);
    return str;
}

void Configure::setValue(QString key, QVariant value) {
    cmap.insert(key, value);
//    PrintUtil::printSeperator("set");
//    PrintUtil::print(key);
//    PrintUtil::print(value.toString());
//    PrintUtil::printSeperator("set");
}

void Configure::readConfig() {
    QString configures = FileUtil::readAll(CONFIGURE_FILE);
    QStringList csl = configures.split(FileUtil::NEW_LINE);
    for(int i=0; i < csl.count(); i++) {
        QString line = csl.at(i);
        QStringList tmplist = line.split(CONFIGURE_SEPE_FLAG);
        if(tmplist.count() == 2) {
            cmap.insert(tmplist.at(0), QVariant(tmplist.at(1)));
        }
    }
}

bool Configure::writeConfig() {
    /* 写入配置文件中 */
   return writeConfig(toString());
}

bool Configure::writeConfig(QString content) {
    return FileUtil::write(CONFIGURE_FILE, content);
}

bool Configure::resetConfigFile() {
    writeConfig("");
}
