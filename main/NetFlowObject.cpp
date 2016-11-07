#include "NetFlowObject.h"
#include "../utils/PrintUtil.h"

NetFlowObject::NetFlowObject(QString ifname) : ifname(ifname) {
    recv_bytes = 0;
    trans_bytes = 0;
    last_recv_bytes = 0;
    last_trans_bytes = 0;
}
NetFlowObject::NetFlowObject() {

}

NetFlowObject::~NetFlowObject() {

}

void NetFlowObject::setIfName(QString ifname) {
    this->ifname = ifname;
}

void NetFlowObject::updateRecvBytes(int recv_bytes) {
    this->last_recv_bytes = this->recv_bytes;
//    PrintUtil::print(ifname + " === 111 === " + QString::number(last_recv_bytes) + "  " + QString::number(this->recv_bytes));
    this->recv_bytes = recv_bytes;
//    PrintUtil::print(ifname + " === 222 === " + QString::number(this->recv_bytes) + "  " + QString::number(recv_bytes));
}

int NetFlowObject::recvBytesChanged() {
    // 刚开始启动程序时，会直接读取 dev 文件导致数据错误。
    if(this->last_recv_bytes == 0)
        return 0;
    return (this->recv_bytes - this->last_recv_bytes);
}

void NetFlowObject::updateTransBytes(int trans_bytes) {
    last_trans_bytes = this->trans_bytes;
    this->trans_bytes = trans_bytes;
}

int NetFlowObject::transBytesChanged() {
    if(this->last_trans_bytes == 0)
        return 0;
    return (this->trans_bytes - this->last_trans_bytes);
}

int NetFlowObject::getLatestRecvBytes() {
    return recv_bytes;
}

int NetFlowObject::getLatestTransBytes() {
    return trans_bytes;
}

QString NetFlowObject::getIfName() {
    return ifname;
}

QString NetFlowObject::toString() {
    QString str;
    str = "Interface name:" + ifname + ", Receive bytes: " +QString::number(recv_bytes) + ", Transmit bytes: " + QString::number(trans_bytes);
    return str;
}

bool NetFlowObject::operator==(QString ifname) {
    return this->ifname == ifname;
}

bool NetFlowObject::operator==(const NetFlowObject &nfo) {
    return this->ifname == nfo.ifname;
}
