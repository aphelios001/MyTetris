#include <QDebug>
#include "GameScenePainter.h"
#include "ui_GameScenePainter.h"
#include "mainwindow.h"

GameScenePainter::GameScenePainter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameScenePainter)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowFlags(Qt::FramelessWindowHint);//隐藏最大最小化等按键
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明化
    this->setFocus();//设置焦点

    //gameQTimer = new QTimer(this);
    //paintQTimer = new QTimer(this);
    //connect(gameQTimer, &QTimer::timeout, this, &GameScenePainter::BlockUpdate);
    //connect(paintQTimer, &QTimer::timeout, this, &GameScenePainter::SceneUpdate);

    //初始化游戏结束窗口
    gameOverMessage = new QMessageBox();
    yesButton = gameOverMessage->addButton("重新开始", QMessageBox::YesRole);
    noButton = gameOverMessage->addButton("退出游戏", QMessageBox::NoRole);
    gameOverMessage->setWindowTitle("游戏结束");

    connect(ui->SaveButton, SIGNAL(released()), this, SLOT(SaveButtonReleased()));


}

void GameScenePainter::startGame(const QString& filename)
{

    //设定游戏初始状态
    blocks = new Block();
    Score = 0;
    isGameOver = false;

    if(!LoadFlag)
    {
        //初始化方块
        blocks->InitScene();
        blocks->GetNextBlock();
        blocks->CreateNewBlock();
    }
    else
    {
        if(!loadGame(filename))
        {
            qDebug() << "读取失败，默认创建新游戏！";
            blocks->InitScene();
            blocks->GetNextBlock();
            blocks->CreateNewBlock();
        }
        else
        {
            qDebug() << "读取成功！";
        }
    }

    ui->SpeedLine->setText(QString::number(speed_ms) + "ms");
    ui->ScoreLine->setText(QString::number(Score));

    //gameQTimer->start(speed_ms);
    //paintQTimer->start(refresh_ms);

    //开启计时器
    gameTimer = startTimer(speed_ms);//游戏计时器
    paintTimer = startTimer(refresh_ms);//画面刷新计数器


}

void GameScenePainter::gameOver()//游戏结束
{

    killTimer(gameTimer);
    killTimer(paintTimer);

    //gameQTimer->stop();
    //paintQTimer->stop();

    //弹出窗口
    gameOverMessage->setText(QString("游戏结束，你的得分为 %1").arg(Score));
    gameOverMessage->exec();
    if(gameOverMessage->clickedButton() == yesButton)
    {
        startGame("");
    }
    else if(gameOverMessage->clickedButton() == noButton)
    {
        this->close();
    }


}

void GameScenePainter::MoveDownFaster()//快速下落
{
    int newSpeed = speed_ms / 4;
    gameTimer = startTimer(newSpeed);

    //gameQTimer->setInterval(newSpeed);
}

void GameScenePainter::MoveDownOrigin()//恢复原来的下落速度
{
    gameTimer = startTimer(speed_ms);

    //gameQTimer->setInterval(speed_ms);
}

void GameScenePainter::setGameSpeed(int speed)
{
    speed_ms = speed;
}

void GameScenePainter::setLoadFlag(bool loadflag)
{
    LoadFlag = loadflag;
}

void GameScenePainter::pauseGame()
{
    if(!isPause)
    {
        savedGameTimer = speed_ms;
        savedPaintTimer = refresh_ms;

        killTimer(gameTimer);
        killTimer(paintTimer);

        isPause = true;
    }
}

void GameScenePainter::resumeGame()
{
    if(isPause)
    {
        gameTimer = startTimer(savedGameTimer);
        paintTimer = startTimer(savedPaintTimer);

        isPause = false;

        update();
    }
}

bool GameScenePainter::saveGame(const QString &filename)
{
    bool res = GameSaveAndLoad::saveGame(filename, *blocks, Score, speed_ms);
    return res;
}

bool GameScenePainter::loadGame(const QString &filename)
{
    int savedScore = 0;
    int savedSpeed = DEFAULT_SPEED_MS;

    if(GameSaveAndLoad::loadGame(filename, *blocks, savedScore, savedSpeed))
    {
        Score = savedScore;
        speed_ms = savedSpeed;
        setGameSpeed(speed_ms);

        update();
        return true;
    }

    return false;
}


GameScenePainter::~GameScenePainter()
{
    delete ui;
}

void GameScenePainter::timerEvent(QTimerEvent *event)//计时器更新画面
{
    if(event->timerId() == gameTimer)
    {
        ui->ScoreLine->setText(QString::number(Score));

        bool res = blocks->AutoMoveDown();//方块自动下落
        if(!res)//不能再下落了
        {
            blocks->MergeSceneAndBlock(Score);//合并方块

            isGameOver = blocks->isGameOver();
            if(isGameOver)//判断游戏是否结束
            {
                update();
                gameOver();
                update();
            }
            else
            {
                blocks->CreateNewBlock();
            }
        }
    }

    //更新画面
    if(event->timerId() == paintTimer)
    {
        update();
    }
}

void GameScenePainter::paintEvent(QPaintEvent *event)//绘制画面
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);


    //绘制网格
    QPainter painter(this);

    painter.setPen(QPen(Qt::white,1));

    //画竖线
    /*for(int i = 0; i < ColBlockNum + 1; i++)
    {
        painter.drawLine(Margain + i * BlockSize, Margain, Margain + i * BlockSize, Margain + RowBlockNum * BlockSize);
    }
    //画横线
    for(int j = 0; j < RowBlockNum + 1; j++)
    {
        painter.drawLine(Margain, Margain + j * BlockSize, Margain + ColBlockNum * BlockSize, Margain + j * BlockSize);
    }*/

    for(int i = 0; i < RowBlockNum; i++)
    {
        for(int j = 0; j < ColBlockNum; j++)
        {
            painter.drawPixmap(Margain + j * BlockSize, Margain + i * BlockSize, BlockSize, BlockSize, QPixmap("://image/block/png/element_grey_square_glossy.png"));
        }
    }

    //绘制下一个方块的网格
    //这里的x和y是ui真正的坐标
    //左上角格子的左上角坐标
    int next_block_x = 420;
    int next_block_y = 225;

    for(int realX = next_block_x; realX <= next_block_x + 3 * NextBlockSize; realX += NextBlockSize)
    {
        for(int realY = next_block_y ; realY <= next_block_y + 3 * NextBlockSize; realY += NextBlockSize)
        {
            painter.drawPixmap(realX ,realY , NextBlockSize, NextBlockSize, QPixmap("://image/block/png/element_grey_square_glossy.png"));
        }
    }

    //绘制下一个方块

    for(int block_dy = 0; block_dy < Block::sc_BlockRow; block_dy++)
    {
        for(int block_dx = 0; block_dx < Block::sc_BlockCol; block_dx++)
        {
            int tempNum = 1 << block_dx;
            if( (blocks->next_block[block_dy] & tempNum) == tempNum)
            {
                int next_block_sceneX = next_block_x + (Block::sc_BlockCol - block_dx - 1) * NextBlockSize;
                int next_block_sceneY = next_block_y + block_dy * NextBlockSize;
                painter.drawPixmap(next_block_sceneX, next_block_sceneY, NextBlockSize, NextBlockSize, QPixmap("://image/block/png/element_red_square_glossy.png"));
            }
        }
    }

    //下面的x和y是格子数
    //绘制游戏场景
    painter.setPen(QPen(Qt::black,2));

    int startY = 3;
    int endY = startY + Block::sc_GameRow - 1;//y从3到22 从上往下
    int startX = 3;
    int endX = 12;//x从3到12 从右往左 （1依次左移dx位（dx从小到大），所以是从右往左）

    //每一行 用 1左移dx位 和 场景每一行（也是一个数） 进行 位与 运算 ，如果位与运算结果就等于 1左移dx位 那么就说明
    //此 (dx,dy) 有方块 ， dx表示整个场景（24x16）从右往左数第 dx 个，dy表示整个场景（24x16）从上往下数第 dy 个
    //下面的 sceneX 表示玩家看得见的区域(20x10)从 右往左数 第 x 个数
    //下面的 sceneY 表示玩家看得见的区域(20x10)从 上往下数 第 y 个数 , 详情见UI.drawio和Tetris.drawio

    for(int dy = startY; dy <= endY; dy++)
    {
        for(int dx = startX; dx <= endX; dx++)
        {
            int tempNum = 1 << dx;
            if( (blocks->GameSceneBlock[dy] & tempNum) == tempNum )
            {
                int sceneX = dx - 3 + 1;
                int sceneY = dy - 3 + 1;

                painter.drawPixmap(BlockSize * (10 - sceneX) + Margain, (sceneY - 1) * BlockSize + Margain, BlockSize, BlockSize, QPixmap("://image/block/png/element_blue_square_glossy.png"));
                //painter.setBrush(QBrush(Qt::blue,Qt::SolidPattern));
                //painter.drawRect(BlockSize * (10 - sceneX) + Margain, (sceneY - 1) * BlockSize + Margain, BlockSize, BlockSize);
            }
        }
    }


    //下面的block_dy和block_dx是一个新的坐标系的x和y
    //block_dy代表一个方格(4x4)从上往下第几行(从0-3)
    //block_dx表示一个方格(4x4)从右往左第几列(从0-3)
    //换算成玩家可见区域(20x10)的sceneX 和 sceneY 需要依靠 block 自身的 x 和 y
    //公式如下


    //画方块, 方块要单独画，因为还没合并到游戏场景内，此时方块是独立的，只是依靠block自身的 x 和 y 代表其在场景中的位置
    //同样的，如果用 1左移block_dx位 和  block[block_dy] 进行 位与 ， 如果结果 为 1左移block_dx ，
    //那么就说明 方块坐标系的 (block_dx, block_dy) 有方块
    //换算成玩家可见区域(20x10)的sceneX 和 sceneY 需要依靠 block 自身的 x 和 y

    for(int block_dy = 0; block_dy < Block::sc_BlockRow; block_dy++)
    {
        for(int block_dx = 0; block_dx < Block::sc_BlockCol; block_dx++)
        {
            int tempNum = 1 << block_dx;
            if( (blocks->block[block_dy] & tempNum) == tempNum)
            {
                int sceneX = (blocks->getX() + 1) - Block::sc_BlockCol + (block_dx + 1);
                int sceneY = (blocks->getY() + block_dy) - 3 + 1;
                if(sceneY <= 0)//若sceneY <= 0 不绘制 因为这时还没落下来 还在 dy(0-2)区域内
                {
                    continue;
                }
                //painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
                //painter.drawRect(BlockSize * (10 - sceneX) + Margain, (sceneY - 1) * BlockSize + Margain, BlockSize, BlockSize);
                painter.drawPixmap(BlockSize * (10 - sceneX) + Margain, (sceneY - 1) * BlockSize + Margain, BlockSize, BlockSize, QPixmap("://image/block/png/element_red_square_glossy.png"));
                //painter.drawPixmap(BlockSize * (10 - sceneX) + Margain, (sceneY - 1) * BlockSize + Margain, BlockSize, BlockSize, QPixmap("://image/block/png/selectorC.png"));
            }
        }
    }


}

void GameScenePainter::keyPressEvent(QKeyEvent *event)//按键事件
{
    int key = event->key();

    if(isGameOver)
    {
        return;
    }

    switch(key)
    {
    case Qt::Key_Left: blocks->MoveLeft(); break;
    case Qt::Key_Right: blocks->MoveRight(); break;
    case Qt::Key_Up: blocks->BlockRotate(); break;
    case Qt::Key_Down:
        if(!event->isAutoRepeat())
        {
            MoveDownFaster();
        }
        break;
    }
}

void GameScenePainter::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();

    if(isGameOver)
    {
        return;
    }

    switch(key)
    {
    case Qt::Key_Down:
        if(!event->isAutoRepeat())
        {
            MoveDownOrigin();
        }
        break;
    }
}


void GameScenePainter::on_CloseButton_released()
{
    this->close();
}

void GameScenePainter::SaveButtonReleased()
{
    pauseGame();

    QString filename = QFileDialog::getSaveFileName(this, tr("保存游戏"), QDir::homePath(), tr("俄罗斯方块存档 (*.sav)"));

    if(!filename.isEmpty())
    {
        if(!filename.endsWith(".sav"))
        {
            filename += ".sav";
        }
        saveGame(filename);
    }

    resumeGame();
}



//测试计时器功能用
void GameScenePainter::SceneUpdate()
{
    update();
}

void GameScenePainter::BlockUpdate()
{
    bool res = blocks->AutoMoveDown();//方块自动下落
    if(!res)//不能再下落了
    {
        ui->ScoreLine->setText(QString::number(Score));
        blocks->MergeSceneAndBlock(Score);//合并方块

        if(blocks->isGameOver())//判断游戏是否结束
        {
            isGameOver = true;
            update();
            gameOver();
            update();
        }
        else
        {
            blocks->CreateNewBlock();
        }
    }
}

