#ifndef BLOCK_H
#define BLOCK_H

#include <QRandomGenerator>
#include <QDataStream>
#include "Tetrominos.h"

class Block
{
public:
    static constexpr int sc_ArrRow = 24;//行数
    static constexpr int sc_ArrCol = 16;//列数
    static constexpr int sc_GameRow = 20;//游戏实际行数
    static constexpr int sc_GameCol = 10;//游戏实际列数

    static constexpr int sc_FullRow = 0b1111111111111111;//满行，用于消除

    static constexpr int sc_BlockRow = Tetrominos::sc_Row;//一个方块的行数
    static constexpr int sc_BlockCol = Tetrominos::sc_Row;//一个方块的列数

    static constexpr int DefaultSceneBlock[sc_ArrRow] =
    {
        0b1110000000000111,//0
        0b1110000000000111,
        0b1110000000000111,
        0b1110000000000111,//3实际看到的行开始
        0b1110000000000111,
        0b1110000000000111,//5
        0b1110000000000111,
        0b1110000000000111,
        0b1110000000000111,
        0b1110000000000111,
        0b1110000000000111,//10
        0b1110000000000111,
        0b1110000000000111,
        0b1110000000000111,
        0b1110000000000111,
        0b1110000000000111,//15
        0b1110000000000111,
        0b1110000000000111,
        0b1110000000000111,
        0b1110000000000111,
        0b1110000000000111,//20
        0b1110000000000111,
        0b1110000000000111,//22实际看到的行的结束，共22 - 3 + 1 = 20行
        0b1111111111111111,//23
    };

    int GameSceneBlock[sc_ArrRow];

    int block[sc_BlockRow];
    int next_block[sc_BlockRow];

    friend QDataStream& operator <<(QDataStream &out, const Block &block);
    friend QDataStream& operator >>(QDataStream &in, Block &block);


    int getY();

    int getX();

    void InitScene();

    void GetNextBlock();

    void CreateNewBlock();

    void MergeSceneAndBlock(int &Score);

    bool isBlockCollide();

    bool AutoMoveDown();

    void MoveLeft();

    void MoveRight();

    void BlockRotate();

    bool isGameOver();

    void clearBlock();

private:

    //当前方块的状态，（位置，类型，下一个方块类型）
    //方块在场景中的位置，需要通过当前的x 和 y来确定。
    //注意：block[i]只占用低4位，低4位左移 x 位后，低位会补0，原本高位也都是0，因此只剩下这4位的数据存在。
    int x = 5;
    int y = 0;
    int CurrentBlockType = 0;
    int NextBlockType = 0;
    int BlockState = 0;
};

#endif // BLOCK_H
