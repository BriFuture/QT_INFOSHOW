#ifndef FloatWidget_H
#define FloatWidget_H

#include <QWidget>
#include <QStringList>
#include <QMouseEvent>
#include <QPoint>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include "../main/NetInfo.h"

namespace Ui {
class FloatWidget;
}

class FloatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FloatWidget(QWidget *parent = 0);
    ~FloatWidget();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    Ui::FloatWidget *ui;
    NetInfo *ni;
    QStringList netiflist;
    QPoint inPoint;  // 窗口内部坐标
    void initUI();
    QPixmap bg_img;
    QString bg_img_path;

public slots:
    void updateNetInfo();
    void updateNetList();
    void setBackgroundImg(QString imgpath);

};

#endif // FloatWidget_H
