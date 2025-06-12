#ifndef GAMESCENEPAINTER_H
#define GAMESCENEPAINTER_H

#include <QWidget>
#include <QMessageBox>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QStyleOption>
#include <QPen>
#include <QFileDialog>
#include "Block.h"
#include "GameSaveAndLoad.h"

constexpr int DEFAULT_SPEED_MS = 400;
constexpr int REFRESH_MS = 1;

constexpr int Margin = 20;
constexpr int BlockSize = 30;
constexpr int NextBlockSize = 20;
constexpr int RowBlockNum = 20;
constexpr int ColBlockNum = 10;

namespace Ui {
class GameScenePainter;
}

class GameScenePainter : public QWidget
{
    Q_OBJECT

public:
    explicit GameScenePainter(QWidget *parent = nullptr);
    void startGame(const QString& filename);
    void gameOver();
    void MoveDownFaster();
    void MoveDownOrigin();

    void setGameSpeed(int speed);
    void setLoadFlag(bool loadflag);

    void pauseGame();
    void resumeGame();

    bool saveGame(const QString& filename);
    bool loadGame(const QString& filename);

    ~GameScenePainter();

private:
    Ui::GameScenePainter *ui;

    QMessageBox *gameOverMessage;
    QPushButton *yesButton;
    QPushButton *noButton;

    //Block 实例
    Block *blocks;

    //分数
    int Score = 0;

    //标志
    bool isGameOver = false;
    bool LoadFlag = false;
    bool isPause = false;

    //游戏速度 和 画面刷新速度
    int speed_ms = DEFAULT_SPEED_MS;
    int refresh_ms = REFRESH_MS;

    int savedGameTimer = DEFAULT_SPEED_MS;
    int savedPaintTimer = REFRESH_MS;


    //计时器相关变量
    int gameTimer;
    int paintTimer;


//各种事件重写声明
protected:
    void timerEvent(QTimerEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

//槽函数声明
private slots:
    void on_CloseButton_released();
    void SaveButtonReleased();
    void PARButtonReleased();// PAR : Pause And Resume

};

#endif // GAMESCENEPAINTER_H
