#include "FloatWidget.h"
#include "ui_FloatWidget.h"
#include "../utils/PrintUtil.h"
#include <QCursor>
#include <QPalette>
#include <QColor>

#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QIcon>

const QString FloatWidget::CONFIG_PREFIX = "float_window_";

FloatWidget::FloatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatWidget), ni(new NetInfo)
{
    ui->setupUi(this);
//    ni->moveToThread(&nithread);

    QObject::connect(ni, SIGNAL(infoUpdate()), this, SLOT(updateNetInfo()));
    QObject::connect(ni, SIGNAL(netlistUpdate()), this, SLOT(updateNetList()));
    initFromConfig();
    initUI();
}

FloatWidget::~FloatWidget()
{
    delete ui;
    delete ni;
}

void FloatWidget::initFromConfig() {
    config = Configure::getConfigure();
    config->readConfig();
    int px = config->getValue(CONFIG_PREFIX + "x", 0).toInt();
    int py = config->getValue(CONFIG_PREFIX + "y", 0).toInt();
    bg_img_path = config->getValue(CONFIG_PREFIX + "bg_img", ":/res/bg.png").toString();
    opacity = config->getValue(CONFIG_PREFIX + "opacity", QString::number(80)).toInt();
    isontop = config->getValue(CONFIG_PREFIX + "ontop", true).toBool();
    select_img = ":/res/select.png";
//    PrintUtil::print(opacity, "init from config");
    this->setWindowOpacity((opacity+0.0)/100);
    this->move(px, py);
}

void FloatWidget::initUI() {
    // 设置窗体 Flag, 无标题最大最小化按钮
    setWindowFlags(Qt::FramelessWindowHint);
    /* 不设置 Qt::X11BypassWindowManagerHint 标志，否则无法正常设置置顶或取消置顶 **/
    if(isontop)
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
//        raise();


    setAutoFillBackground(true);
    //设置背景
//    setAttribute(Qt::WA_TranslucentBackground, true);
//    setStyleSheet("background-image:url(:/res/bg.png);");
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
    tmpPainter.fillRect(bgimg.rect(), QColor(0, 0, 0, 156));  // 设置背景透明度
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
        popUpMenu();
    }
}

void FloatWidget::popUpMenu() {
    QMenu popmenu(this);
    //设置置顶
    QAction ontop_action(&popmenu);
    ontop_action.setText("置顶");

    if(isontop)
        ontop_action.setIcon(QIcon(select_img));

    // 设置透明度菜单
    QMenu opacitySubMenu(&popmenu);
    QActionGroup opacityGroup(&popmenu);
    opacitySubMenu.setTitle("透明度");
    QAction opacity_action_100("100%", &popmenu);
    opacity_action_100.setData(100);
    QAction opacity_action_80("80%", &popmenu);
    opacity_action_80.setData(80);
    QAction opacity_action_60("60%", &popmenu);
    opacity_action_60.setData(60);
    QAction opacity_action_40("40%", &popmenu);
    opacity_action_40.setData(40);
    // 设置选中透明度
    switch(opacity) {
    case 100:
//        opacity_action_100.setText("100% ○");
        opacity_action_100.setIcon(QIcon(select_img));
        break;
    case 60:
//        opacity_action_60.setText("60% ○");
        opacity_action_60.setIcon(QIcon(select_img));
        break;
    case 40:
//        opacity_action_40.setText("40% ○")
        opacity_action_40.setIcon(QIcon(select_img));;
        break;
    case 80:
    default:
//        opacity_action_80.setText("80% ○");
        opacity_action_80.setIcon(QIcon(select_img));
        break;
    }

    opacityGroup.addAction(&opacity_action_100);
    opacityGroup.addAction(&opacity_action_80);
    opacityGroup.addAction(&opacity_action_60);
    opacityGroup.addAction(&opacity_action_40);
    opacitySubMenu.addActions(opacityGroup.actions());

    // 退出按钮
    QAction quit_action("退出", &popmenu);

    //添加菜单项
    popmenu.addAction(&ontop_action);
    popmenu.addMenu(&opacitySubMenu);
    popmenu.addAction(&quit_action);

    // 连接信号和槽
    QObject::connect(&ontop_action, SIGNAL(triggered(bool)), this, SLOT(windowOnTop(bool)));
    QObject::connect(&quit_action, &QAction::triggered, this, &QWidget::close);
    QObject::connect(&opacityGroup, SIGNAL(triggered(QAction *)), this, SLOT(setOpacity(QAction *)));
    // 设置弹出菜单位置
    popmenu.exec(cursor().pos());
}

void FloatWidget::setOpacity(QAction *action) {
//    PrintUtil::print(action->data().toInt(), "opacity");
    opacity = action->data().toInt();
    double opac = (opacity + 0.0) / 100;
    setWindowOpacity(opac);
//    PrintUtil::print(opac, "set opacity");
    config->setValue(CONFIG_PREFIX + "opacity", opacity);
}

void FloatWidget::windowOnTop(bool) {
    hide();
    if(!isontop) {
        // 设置窗体 Flag
        setWindowFlags(Qt::Widget);
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
//        raise();
//        PrintUtil::print("set on top");
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
//        PrintUtil::print("set not on top");
    }
    isontop = !isontop;
    this->show();
    config->setValue(CONFIG_PREFIX+ "ontop", isontop);
}

void FloatWidget::mouseMoveEvent(QMouseEvent *event) {
//    QPoint temppoint = event->pos();
//    PrintUtil::print("tmp l: "+ QString::number(temppoint.x()) + "  r: "+ QString::number(temppoint.y()));
    QPoint mousePos = cursor().pos();
//    PrintUtil::print("mouse l: "+ QString::number(mousePos.x()) + "  r: "+ QString::number(mousePos.y()));
    int x = mousePos.x()-inPoint.x();
    int y = mousePos.y()-inPoint.y();
    if(x < 0)
        x = 0;
    if(y < 0)
        y = 0;
    this->move(x, y);
    config->setValue(CONFIG_PREFIX + "x", x);
    config->setValue(CONFIG_PREFIX + "y", y);
}

void FloatWidget::closeEvent(QCloseEvent *event) {

    ni->stopRefresh();
    // 将修改的配置写入文件
    config->setValue(CONFIG_PREFIX + "bg_img", bg_img_path);
    if(config->writeConfig())
        PrintUtil::print("配置文件写入完成。");
    else
        PrintUtil::print("无法保存配置文件。");
    event->accept();
    PrintUtil::print("正在关闭悬浮窗。");
}
