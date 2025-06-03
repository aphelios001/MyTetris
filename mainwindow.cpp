#include <QPropertyAnimation>
#include <QLineEdit>
#include <QFileDialog>
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

    game_painter = new GameScenePainter();

    connect(ui->StartButton,SIGNAL(clicked()),this,SLOT(StartButtonClicked()));
    connect(ui->QuitButton,SIGNAL(clicked()),this,SLOT(QuitButtonClicked()));
    connect(ui->SettingButton, SIGNAL(clicked()), this, SLOT(SettingButtonClicked()));
    connect(ui->LoadButton, SIGNAL(clicked()), this, SLOT(LoadButtonClicked()));
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

//开始游戏按钮按下
void MainWindow::StartButtonClicked()
{
    game_painter->startGame("");//开始游戏
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

void MainWindow::SettingButtonClicked()
{
    // 将 settingWindow 改为 QDialog 类型
    settingWindow = new QDialog(this, Qt::FramelessWindowHint);
    settingWindow->setFixedSize(400, 300);

    // 设置样式（浅色渐变背景）
    settingWindow->setStyleSheet(QString::fromUtf8("QDialog{"
                                                   "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                                   "                                 stop:0 #e0f7fa,"
                                                   "                                 stop:1 #ffeef0);"
                                                   "    border-radius: 10px;"
                                                   "    border-style:solid;"
                                                   "    border-width:2px;"
                                                   "    border-color:rgb(213, 213, 213);"
                                                   "}"));


    QLabel *speedLabel = new QLabel("游戏速度:", settingWindow);
    QLineEdit *speedLineEdit = new QLineEdit(settingWindow);
    speedLineEdit->setPlaceholderText("输入游戏速度（单位：ms）");
    speedLineEdit->setValidator(new QIntValidator(10, 10000, this));

    QPushButton *confirmButton = new QPushButton("确认", settingWindow);
    QPushButton *cancelButton = new QPushButton("取消", settingWindow);


    confirmButton->setStyleSheet("QPushButton { padding: 10px; border-radius: 5px; background: #a8e6cf; }");
    cancelButton->setStyleSheet("QPushButton { padding: 10px; border-radius: 5px; background: #ffc1c1; }");


    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(speedLabel);
    inputLayout->addWidget(speedLineEdit);


    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);


    QVBoxLayout *mainLayout = new QVBoxLayout(settingWindow);
    mainLayout->addStretch();
    mainLayout->addLayout(inputLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(20, 20, 20, 20);


    connect(confirmButton, &QPushButton::clicked, [&]() {
        QString text = speedLineEdit->text();
        if (!text.isEmpty()) {
            int speed = text.toInt();
            game_painter->setGameSpeed(speed);
        }
        settingWindow->accept();
    });


    connect(cancelButton, &QPushButton::clicked, [&]() {
        settingWindow->reject();
    });


    if (settingWindow->exec() == QDialog::Accepted) {
        qDebug() << "用户点击了确认";
    } else {
        qDebug() << "用户点击了取消或关闭";
    }
}

void MainWindow::LoadButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("加载游戏"),
                                                    QDir::homePath(),
                                                    tr("俄罗斯方块存档 (*.sav)"));

    if (!filename.isEmpty()) {
        game_painter->setLoadFlag(true);
    }

    game_painter->startGame(filename);
    game_painter->show();
    this->close();
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

