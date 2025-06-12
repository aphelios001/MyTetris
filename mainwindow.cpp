#include <QPropertyAnimation>
#include <QLineEdit>
#include <QFileDialog>
#include <QFormLayout>
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

    //窗口出现动画实现
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(500);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    game_painter = new GameScenePainter();

    //连接信号与槽
    connect(ui->StartButton, SIGNAL(clicked()),this, SLOT(StartButtonClicked()));
    connect(ui->QuitButton, SIGNAL(clicked()),this, SLOT(QuitButtonClicked()));
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

//以下三个鼠标事件的重写: 实现窗口拖拽
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
    game_painter->startGame("");//开始游戏, filename为空
    game_painter->show();
    this->close();
}

//窗口退出动画实现
void MainWindow::QuitButtonClicked()
{
    QPropertyAnimation *animation= new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation,SIGNAL(finished()), this, SLOT(close()));
}

//加载按钮按下
void MainWindow::LoadButtonClicked()
{
    // 弹出文件选择对话框，让用户选择要加载的游戏存档文件
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("加载游戏"),           // 对话框标题
                                                    QDir::homePath(),         // 默认打开路径：用户主目录
                                                    tr("俄罗斯方块存档 (*.sav)")); // 文件过滤器

    // 如果用户选择了非空的文件路径
    if (!filename.isEmpty()) {
        // 设置加载标志为 true，表示即将加载游戏存档
        game_painter->setLoadFlag(true);

        // 调用 startGame 函数并传入文件名，开始加载游戏
        game_painter->startGame(filename); // filename 是所选文件名

        // 显示游戏界面
        game_painter->show();

        // 关闭当前窗口
        this->close();
    }
}

//设置按钮按下
void MainWindow::SettingButtonClicked()
{

    // 创建设置窗口
    settingWindow = new QDialog(this, Qt::FramelessWindowHint);
    settingWindow->setFixedSize(500, 400);

    // 设置窗口样式（浅色渐变背景 + 圆角边框）
    settingWindow->setStyleSheet(QString::fromUtf8("QDialog{"
                                                   "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                                   "                                 stop:0 #f0f8ff,"
                                                   "                                 stop:1 #fff0f5);"
                                                   "    border-radius: 15px;"
                                                   "    border-style:solid;"
                                                   "    border-width:2px;"
                                                   "    border-color:rgb(200, 200, 200);"
                                                   "}"));

    // 标签和输入框
    QLabel *speedLabel = new QLabel("游戏速度:", settingWindow);
    speedLabel->setStyleSheet("font-size: 14px; font-family: Arial, sans-serif;");

    QLineEdit *speedLineEdit = new QLineEdit(settingWindow);
    speedLineEdit->setPlaceholderText("输入游戏速度(100ms - 2000ms之间)");
    speedLineEdit->setValidator(new QIntValidator(100, 2000, this));
    speedLineEdit->setStyleSheet(QString::fromUtf8(
        "padding: 8px;"
        "border-radius: 5px;"
        "border: 1px solid #ccc;"
        "font-size: 14px;"
        "background: #ffffff;"
        "selection-background-color: #a8e6cf;"
        ));

    // 确认和取消按钮
    QPushButton *confirmButton = new QPushButton("确认", settingWindow);
    QPushButton *cancelButton = new QPushButton("取消", settingWindow);

    // 按钮样式（带悬停和点击效果）
    confirmButton->setStyleSheet(QString::fromUtf8(
        "QPushButton {"
        "    padding: 10px 20px;"
        "    border-radius: 5px;"
        "    background: #a8e6cf;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: #8cd9b3;"
        "}"
        "QPushButton:pressed {"
        "    background: #6ec699;"
        "}"));

    cancelButton->setStyleSheet(QString::fromUtf8(
        "QPushButton {"
        "    padding: 10px 20px;"
        "    border-radius: 5px;"
        "    background: #ffc1c1;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: #e6a9a9;"
        "}"
        "QPushButton:pressed {"
        "    background: #cc9090;"
        "}"));

    // 输入布局
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    inputLayout->addRow(speedLabel, speedLineEdit);

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(settingWindow);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);
    mainLayout->addStretch();
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    // 连接信号与槽
    connect(confirmButton, &QPushButton::clicked, [&]() {
        QString text = speedLineEdit->text();
        if (!text.isEmpty()) {
            int speed = text.toInt();
            // 添加额外判断，确保速度在 100 到 2000 之间
            if (speed >= 100 && speed <= 2000) {
                game_painter->setGameSpeed(speed);
            } else {
                QMessageBox::warning(settingWindow, "输入错误", "游戏速度必须在 100 到 2000 之间！", QMessageBox::Ok);
            }
        }
        settingWindow->accept();
    });

    connect(cancelButton, &QPushButton::clicked, [&]() {
        settingWindow->reject();
    });

    // 显示对话框并处理结果
    if (settingWindow->exec() == QDialog::Accepted) {
        qDebug() << "设置窗口：用户点击了确认";
    } else {
        qDebug() << "设置窗口：用户点击了取消或关闭";
    }
}

//关闭淡出动画实现
void MainWindow::on_CloseButton_released()
{
    QPropertyAnimation *animation= new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation,SIGNAL(finished()), this, SLOT(close()));
}

//主菜单最小化实现
void MainWindow::on_MinimizeButton_released()
{
    this->showMinimized();
}

