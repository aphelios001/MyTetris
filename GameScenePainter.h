#ifndef GAMESCENEPAINTER_H
#define GAMESCENEPAINTER_H

#include <QWidget>
#include <QMessageBox>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QStyleOption>
#include <QPen>
#include "Block.h"

#define DEFAULT_SPEED_MS 400
#define REFRESH_MS 1
constexpr int Margain = 20;
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
    void startGame();
    void gameOver();
    void MoveDownFaster();
    void MoveDownOrigin();
    ~GameScenePainter();

private:
    Ui::GameScenePainter *ui;
    Block *blocks;
    QMessageBox *gameOverMessage;
    QPushButton *yesButton;
    QPushButton *noButton;

    int Score = 0;

    bool isGameOver = false;


    int gameTimer;
    int paintTimer;
    QTimer *gameQTimer;
    QTimer *paintQTimer;
    int speed_ms = DEFAULT_SPEED_MS;
    int refresh_ms = REFRESH_MS;


protected:
    void timerEvent(QTimerEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void on_CloseButton_released();
    void SceneUpdate();
    void BlockUpdate();

};

#endif // GAMESCENEPAINTER_H
