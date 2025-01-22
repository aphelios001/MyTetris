#include <QPropertyAnimation>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MenuWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());//固定窗口大小
    this->setWindowFlags(Qt::FramelessWindowHint);//隐藏最大最小化等按键
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明化

    //窗口出现动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(500);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    connect(ui->StartButton,SIGNAL(clicked()),this,SLOT(StartButtonClicked()));
    connect(ui->QuitButton,SIGNAL(clicked()),this,SLOT(QuitButtonClicked()));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

//三个鼠标事件的重写
//鼠标释放
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)	//鼠标左键释放
        last = event->globalPos();
}
//鼠标移动
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()==Qt::LeftButton)	//坐标左键按下并移动
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }

}
//鼠标按下
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)	//鼠标左键按下
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        move(x()+dx, y()+dy);
    }

}


void MainWindow::StartButtonClicked()
{
    game_painter = new GameScenePainter();
    game_painter->show();
    this->close();
}

//窗口退出动画
void MainWindow::QuitButtonClicked()
{
    QPropertyAnimation *animation= new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation,SIGNAL(finished()), this, SLOT(close()));
}

void MainWindow::on_CloseButton_released()
{
    QPropertyAnimation *animation= new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation,SIGNAL(finished()), this, SLOT(close()));
}

void MainWindow::on_MinimizeButton_released()
{
    this->showMinimized();
}

