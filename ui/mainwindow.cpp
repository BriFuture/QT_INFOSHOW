#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../utils/PrintUtil.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(onpushBtnClick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onpushBtnClick() {
    PrintUtil::print("Push btn clicked!");
}


