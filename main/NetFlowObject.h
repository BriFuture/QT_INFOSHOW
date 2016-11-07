#ifndef NETFLOWOBJECT_H
#define NETFLOWOBJECT_H

#include <QString>

class NetFlowObject {
private:
    QString ifname;
    int recv_bytes;
    int trans_bytes;
    int last_recv_bytes;
    int last_trans_bytes;
    int recv_packets;
    int trans_packets;

public:
    NetFlowObject(QString ifname);
    NetFlowObject();
    ~NetFlowObject();
    void setIfName(QString ifname);
    /**
     * @brief 更新接收/发送的字节数
     * @param recv_bytes
     * @return
     */
    void updateRecvBytes(int recv_bytes);
    void updateTransBytes(int trans_bytes);
    /**
     * @brief 返回与上一次接收/发送的字节数的差值
     * @return
     */
    int recvBytesChanged();
    int transBytesChanged();
    /**
     * @brief 返回总的字节数
     * @return
     */
    int getLatestRecvBytes();
    int getLatestTransBytes();
    QString getIfName();
    QString toString();
    bool operator ==(QString ifname);
    bool operator ==(const NetFlowObject &nfo);
};

#endif // NETFLOWOBJECT_H
