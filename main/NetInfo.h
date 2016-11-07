#ifndef NETINFO_H
#define NETINFO_H

#include <QObject>
#include <QList>
#include <QStringList>
#include "NetFlowObject.h"
#include <QTimer>

class NetInfo : public QObject {
    Q_OBJECT

public:
    // Net Dev 文件
    static const QString FILE_NET_DEV;
    static const QString ETH_ALL;
    static const int REFRESH_RATE;
private:
    const int ETHER_NAME_COL = 0;
    const int RECEIVE_BYTES_COL = 1;
    const int TRANSMIT_BYTES_COL = 9;
    static const int KBYTES = 1024;
    static const int MBYTES = KBYTES * 1024;
    static const int GBYTES = MBYTES * 1024;
//    QStringList interfaces;
    QList<NetFlowObject>  netflowobj_list;
    QTimer * timer;

private:
    /**
     * 获取特定网络接口（除 lo 外）的字节数
     * 若 eth == NetInfo::ETH_ALL, 返回所有端口的流量
    **/
    int getNetRecvBytes(QString eth);
    int getNetTransBytes(QString eth);

    /**
     * @brief 获取字节数的增量
     * 若 eth == NetInfo::ETH_ALL, 返回所有端口的流量
     */
    int getRecvBytesInc(QString eth);
    int getTransBytesInc(QString eth);

public:
    NetInfo();
    ~NetInfo();

    /** 将字节数转换成合适的单位 **/
    static QString formatData(float bytes);

    /** 从 list 列表中找出网卡名为 ifname 的 NetFlowObject 对象 **/
    bool getNfoFromList(QString ifname, NetFlowObject **nfo);

    /**
     * @brief  计算得到当前网速
     * @return  返回一个二维 float 数组 ，
     *          [0] 为接收的速率 [1] 为发送的速率
     */
//    static float ** getSpeed();

    /**
     * @brief 自动将端口流量转换成易读的形式
     * @param eth
     * @return 获取某个端口上或所有端口（除 lo 外）的流量
     *      若 eth == NetInfo::ETH_ALL, 返回所有端口的流量
     */
    QString getNetFlowData(QString eth, bool recv=true);

    /**
     * @brief 获取网速
     * @param eth 网卡名
     * @return
     *      若 eth == NetInfo::ETH_ALL, 返回所有端口的流量
     */
    QString getSpeed(QString eth, bool recv=true);

    /** 获取所有网络接口，除 lo 外 **/
    QStringList getInterfaces();

public slots:
    /** 从 /proc/net/dev 数据中刷新网络数据 **/
    void refreshNetInfo();
    void stopRefresh();

signals:
    // 数据更新信号
    void infoUpdate();
    // 发现新网卡信号
    void netlistUpdate();
};

#endif // NETINFO_H
