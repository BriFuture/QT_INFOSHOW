#include "NetInfo.h"
#include "../utils/FileUtil.h"
#include "../utils/PrintUtil.h"

const QString NetInfo::ETH_ALL = "\\*ALL_ETH*\\";
const QString NetInfo::FILE_NET_DEV = "/proc/net/dev";
const int NetInfo::REFRESH_RATE = 1200;  // 刷新速率，单位 ms

NetInfo::NetInfo() {
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(refreshNetInfo()));
    timer->start(NetInfo::REFRESH_RATE);
}

NetInfo::~NetInfo() {

}

QStringList NetInfo::getInterfaces() {
    QStringList interfaceList;
    foreach (NetFlowObject nfo, netflowobj_list) {
//        PrintUtil::print("网卡： " + nfo.getIfName());
        interfaceList.append(nfo.getIfName());
    }

    return interfaceList;
}

// ==============  计算网络流量  ==================
// 获取接收字节数
int NetInfo::getNetRecvBytes(QString eth) {
    if(eth == NetInfo::ETH_ALL) {
        int recv_data = 0;
        foreach (NetFlowObject nfo, netflowobj_list) {
            recv_data += nfo.getLatestRecvBytes();
        }
        return recv_data;
    } else {
        NetFlowObject *nfo;
        bool find = getNfoFromList(eth, &nfo);
        if(find)
            return nfo->getLatestRecvBytes();
    }

    return 0;
}

// 获取发送字节数
int NetInfo::getNetTransBytes(QString eth) {
    if(eth == NetInfo::ETH_ALL) {
        int trans_data = 0;
        foreach (NetFlowObject nfo, netflowobj_list) {
            trans_data += nfo.getLatestTransBytes();
        }
        return trans_data;
    } else {
        NetFlowObject *nfo;
        bool find = getNfoFromList(eth, &nfo);
        if(find)
            return nfo->getLatestTransBytes();
    }

    return 0;
}

QString NetInfo::getNetFlowData(QString eth, bool recv) {
    if(recv)
        return formatData(getNetRecvBytes(eth) + 0.0L);
    else
        return formatData(getNetTransBytes(eth) + 0.0L);
}

//==============  计算网速  =====================

int NetInfo::getRecvBytesInc(QString eth) {
    if(eth == NetInfo::ETH_ALL) {
        int recv_bytes_inc = 0;
        foreach (NetFlowObject nfo, netflowobj_list) {
            recv_bytes_inc += nfo.recvBytesChanged();
        }
        return recv_bytes_inc;
    } else {
        NetFlowObject *nfo;
        bool find = getNfoFromList(eth, &nfo);
        if(find)
            return nfo->recvBytesChanged();
    }

    return 0;
}

int NetInfo::getTransBytesInc(QString eth) {
    if(eth == NetInfo::ETH_ALL) {
        int recv_bytes_inc = 0;
        foreach (NetFlowObject nfo, netflowobj_list) {
            recv_bytes_inc += nfo.transBytesChanged();
        }
        return recv_bytes_inc;
    } else {
        NetFlowObject *nfo;
        bool find = getNfoFromList(eth, &nfo);
        if(find)
            return nfo->transBytesChanged();
    }

    return 0;
}

QString  NetInfo::getSpeed(QString eth, bool recv) {
    float speed = 0;
    if(recv)
        speed = (getRecvBytesInc(eth) + 0.0L) / REFRESH_RATE * 1000;  // 刷新速率为 ms
    else
        speed = (getTransBytesInc(eth) + 0.0L) / REFRESH_RATE * 1000;

    return formatData(speed) +"/s";
}

// ======== 格式化输出 ===========
QString NetInfo::formatData(float bytes) {
    QString unit;
    float data = bytes;

    if(bytes > GBYTES) {
        unit = "GB";
        data = data / GBYTES;
    } else if(bytes > MBYTES) {
        unit = "MB";
        data = data / MBYTES;
    } else if (bytes > KBYTES) {
        unit = "Kb";
        data = data / KBYTES;
    } else {
        unit = "b";
    }

    QString datastr;
    datastr.setNum(data, '0', 1);  // 精确到小数点后一位
    return datastr + " " + unit;
}

/** 可以用来获取特定接口的所有数据 **/
bool NetInfo::getNfoFromList(QString ifname,NetFlowObject **nfo) {
    for(int i = 0; i < netflowobj_list.count(); i++) {
        NetFlowObject o = netflowobj_list[i];
//        PrintUtil::print(o.getIfName() + "  ===  " + QString::number(o.getLatestRecvBytes()));
        if(o.getIfName() == ifname) {
            *nfo = &netflowobj_list[i];  // 将指针所指向的 NetFlowObject 指针修改为 QList 中第 i 个对象的引用
            return true;
        }
    }
    return false;
}

void NetInfo::refreshNetInfo() {
    QString netinfo = FileUtil::readAll(FILE_NET_DEV);
    QStringList lineList = netinfo.split("\n");
    // 每次刷新时都要先将 interfaces 数据清空
//    interfaces.clear();
//    PrintUtil::print("刷新网络数据...");
    // 如果发现了新网卡，提示网卡数目。
    bool newif = false;
    for(int i = 0; i < lineList.count(); i++) {
        QString str = lineList.at(i);
        // 如果包含字符 "|" 说明该行为标题，如果包含 "lo:"，说明是本地环回地址，不计流量
        if(str.contains("|") || str.contains("lo:"))
            continue;

        // interfaces 添加该行数据
//        interfaces.append(str);
        QStringList colList = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QString ifname = colList.at(ETHER_NAME_COL);
        ifname = ifname.left(ifname.length() - 1);  // 去掉冒号
        QString if_recv_bytes = colList.at(RECEIVE_BYTES_COL);
        QString if_trans_bytes = colList.at(TRANSMIT_BYTES_COL);
//        PrintUtil::print(ifname);
        // 如果找到相同的 nfo 对象，修改它的数据
        NetFlowObject *nfo;
        bool finded = getNfoFromList(ifname, &nfo);
        if(finded) {
//            PrintUtil::print(nfo->getIfName() + "  "+ QString::number(nfo->getLatestRecvBytes()) + " file bytes:  " + if_recv_bytes);
            nfo->updateRecvBytes(if_recv_bytes.toInt());
            nfo->updateTransBytes(if_trans_bytes.toInt());
            emit infoUpdate();
        } else {
        // list 中没有找到 nfo，添加
            NetFlowObject nfo1(ifname);
            nfo1.updateRecvBytes(if_recv_bytes.toInt());
            nfo1.updateTransBytes(if_trans_bytes.toInt());
            netflowobj_list.append(nfo1);
            newif = true;
            emit netlistUpdate();
        }
    }
    if(newif)
        PrintUtil::print("发现新网卡，共发现 " + QString::number(netflowobj_list.count()) + " 个网卡。");
}

void NetInfo::stopRefresh() {
    timer->stop();
}
