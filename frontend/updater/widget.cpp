#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QImage>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QIcon icon(":/appicon.ico");
    ui->label_2->setPixmap(icon.pixmap(ui->label_2->size()));
    ui->progressBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; background-color: #383838; } QProgressBar::chunk { background-color: #e06900; width: 20px; }");
}

Widget::~Widget()
{
    delete ui;
}

