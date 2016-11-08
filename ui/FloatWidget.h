#ifndef FloatWidget_H
#define FloatWidget_H

#include <QWidget>
#include <QStringList>
#include <QMouseEvent>
#include <QPoint>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QCloseEvent>
#include "../main/NetInfo.h"
#include "../main/Configure.h"

namespace Ui {
class FloatWidget;
}

class FloatWidget : public QWidget
{
    Q_OBJECT
private:
    void initFromConfig();
    void popUpMenu();
    void initUI();
public:
    explicit FloatWidget(QWidget *parent = 0);
    ~FloatWidget();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::FloatWidget *ui;
    NetInfo *ni;
    Configure * config;
    QStringList netiflist;
    QPoint inPoint;  // 窗口内部坐标
    QPixmap bg_img;
    QString bg_img_path;
    double opacity;
    const static QString CONFIG_PREFIX;

public slots:
    void updateNetInfo();
    void updateNetList();
    // 设置背景图片
    void setBackgroundImg(QString imgpath);
    /**
     * 设置透明度
     * @param opacity  透明度， 1 - 100
    **/
    void setOpacity(QAction *action);

};

#endif // FloatWidget_H
