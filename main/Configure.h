#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QString>
#include <QVariant>
#include <QMap>
#include <QStringList>

class Configure {
public:
    const static QString CONFIGURE_FILE;
    const static QString CONFIGURE_SEPE_FLAG;
private:
    QString ckey;
    QVariant cvalue;
    QMap<QString, QVariant> cmap;
    static Configure * config;
private:
    bool writeConfig(QString content);
    Configure();
public:
    ~Configure();
    static Configure * getConfigure();
    /**
     * @brief getAllKeys 获取所有的键
     * @return
     */
    QStringList getAllKeys();
    /**
     * @brief getValue 获取 key 对应的值
     * @param key
     * @return
     */
    QVariant getValue(QString key, QVariant v=QVariant());
    /**
     * @brief setValue  设置 key 对应的值，若没有设置 key，添加
     * @param key
     * @return
     */
    void setValue(QString key, QVariant value);
    /** 写入和读取配置文件 **/
    bool writeConfig();
    void readConfig();
    // 重置配置，清空 Config 文件
    bool resetConfigFile();

    QString toString();

};

#endif // CONFIGURE_H
