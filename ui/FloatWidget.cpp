#include "FloatWidget.h"
#include "ui_FloatWidget.h"
#include "../utils/PrintUtil.h"
#include <QCursor>
#include <QPalette>
#include <QColor>

#include <QMenu>
#include <QAction>

FloatWidget::FloatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatWidget), ni(new NetInfo)
{
    ui->setupUi(this);
    QObject::connect(ni, SIGNAL(infoUpdate()), this, SLOT(updateNetInfo()));
    QObject::connect(ni, SIGNAL(netlistUpdate()), this, SLOT(updateNetList()));

    bg_img_path = ":/res/bg.png";

    initUI();
}

FloatWidget::~FloatWidget()
{
    delete ui;
}

void FloatWidget::initUI() {
//    setWindowFlags(Qt::WindowStaysOnTopHint);  //置顶
//    setWindowFlags(Qt::Widget);  // 取消置顶

    this->setWindowOpacity(0.9);
    // 设置窗体 Flag
    setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
//    setAttribute(Qt::WA_TranslucentBackground, true);
//    setStyleSheet("background-image:url(:/res/bg.png);");
    setAutoFillBackground(true);
    //设置背景
    setBackgroundImg(bg_img_path);
    update();

    ui->recv_label->setText("↓ 0 b/s");
    ui->trans_label->setText("↑ 0 b/s");
    ui->recv_flow_label->setText("0 b");
    ui->trans_flow_label->setText("0 b");
}

void FloatWidget::setBackgroundImg(QString imgpath) {
    // 设置背景图片
    QPixmap bgimg(imgpath);  // 读取背景图片
    bg_img = bgimg;
//    bg_img.scaled(bgimg.size());
    bg_img.fill(Qt::transparent);
    QPainter tmpPainter(&bg_img);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawPixmap(0, 0, bgimg);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    tmpPainter.fillRect(bgimg.rect(), QColor(0, 0, 0, 96));  // 设置背景透明度
    tmpPainter.end();
}

void FloatWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // 从窗口左上角开始绘制图片
    painter.drawPixmap(0,0, bg_img);
}

void FloatWidget::updateNetInfo() {
//    PrintUtil::print("update net info ...");
    QString recvspeed = ni->getSpeed(NetInfo::ETH_ALL);
    QString transspeed = ni->getSpeed(NetInfo::ETH_ALL, false);
    ui->recv_label->setText("↓  " + recvspeed);
    ui->trans_label->setText("↑  " + transspeed);
    ui->recv_flow_label->setText(ni->getNetFlowData(NetInfo::ETH_ALL));
    ui->trans_flow_label->setText(ni->getNetFlowData(NetInfo::ETH_ALL, false));
}

void FloatWidget::updateNetList() {
    netiflist = ni->getInterfaces();
    PrintUtil::print("更新网卡列表，网卡数量： " + QString::number(netiflist.count()));
}

void FloatWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        inPoint = event->pos();
//        PrintUtil::print("tmp l: "+ QString::number(inPoint.x()) + "  r: "+ QString::number(inPoint.y()));
    } else if(event->button() == Qt::RightButton) {
        // 弹出菜单
//        PrintUtil::print("right clicked!");
        QMenu popmenu(this);
        QAction quit_action(this);
        quit_action.setText("退出");
        popmenu.addAction(&quit_action);
        connect(&quit_action, &QAction::triggered, this, &QWidget::close);
        popmenu.exec(cursor().pos());
    }
}


void FloatWidget::mouseMoveEvent(QMouseEvent *event) {
//    QPoint temppoint = event->pos();
//    PrintUtil::print("tmp l: "+ QString::number(temppoint.x()) + "  r: "+ QString::number(temppoint.y()));
    QPoint mousePos = cursor().pos();
//    PrintUtil::print("mouse l: "+ QString::number(mousePos.x()) + "  r: "+ QString::number(mousePos.y()));
    this->move(mousePos.x()-inPoint.x(), mousePos.y()-inPoint.y());
}
