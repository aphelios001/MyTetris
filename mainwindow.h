#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QWidget>
#include "GameScenePainter.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MenuWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void StartButtonClicked();
    void QuitButtonClicked();
    void SettingButtonClicked();
    void LoadButtonClicked();

    void on_CloseButton_released();
    void on_MinimizeButton_released();



private:
    Ui::MenuWindow *ui;
    GameScenePainter *game_painter; //游戏绘制窗口
    QDialog *settingWindow;//设置窗口
    QPoint last;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};
#endif // MAINWINDOW_H
