#ifndef BLOCK_H
#define BLOCK_H

#include "Tetrominos.h"
#include <QRandomGenerator>
class Block
{
public:
    static constexpr int sc_ArrRow = 24;//行数
    static constexpr int sc_ArrCol = 16;//列数
    static constexpr int sc_GameRow = 20;//游戏实际行数
    static constexpr int sc_GameCol = 10;

    static constexpr int sc_BlockRow = Tetrominos::sc_Row;//一个方块的行数
    static constexpr int sc_BlockCol = Tetrominos::sc_Row;

    static constexpr int DefaultSceneBlock[sc_ArrRow] =
    {
            0b1110000000000111,//0
            0b1110000000000111,
            0b1110000000000111,
            0b1110000000000111,
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
            0b1110000000000111,
            0b1111111111111111,//23
    };

    int GameSceneBlock[sc_ArrRow];

    int block[sc_BlockRow];
    int next_block[sc_BlockRow];

private:
    int x = 5;
    int y = 0;
    int BlockType = 0;
    int BlockState = 0;

public:
    int getY()
    {
        return y;
    }
    int getX()
    {
        return x;
    }
    void InitScene()
    {
        for(int i = 0; i < sc_ArrRow; i++)
        {
            GameSceneBlock[i] = DefaultSceneBlock[i];//初始化游戏场景数组
        }
    }
    void GetNextBlock()
    {
        int randomTypeInt = QRandomGenerator::global()->bounded(7);//生成0-6的随机数,代表方块种类
        BlockType = randomTypeInt;
        for(int i = 0; i < sc_BlockRow; i++)
        {
            next_block[i] = Tetrominos::Tetromino[randomTypeInt][BlockState][i];
        }
    }
    void CreateNewBlock()
    {
        x = 5;
        y = 0;
        BlockState = 0;
        for(int i = 0; i < sc_BlockRow; i++)
        {
            block[i] = next_block[i];
        }
        GetNextBlock();
    }
    void MergeSceneAndBlock(int &Score)//maybe have bug
    {
        for(int SceneStartRow = y , BlockStartRow = 0; SceneStartRow < sc_ArrRow && SceneStartRow < y + 4 && BlockStartRow < sc_BlockRow; SceneStartRow++, BlockStartRow++)
        {
            int rowValue = (block[BlockStartRow] << x) | GameSceneBlock[SceneStartRow];
            GameSceneBlock[SceneStartRow] = rowValue;
        }

        //消行
        int cnt = 0;
        for(int Row = 0; Row < sc_GameRow ; Row++) //0-19
        {
            if(GameSceneBlock[Row + 3] == 0b1111111111111111)//3 - 22
            {
                cnt++;
                for(int i = Row + 3; i >= 3; i--)
                {
                    GameSceneBlock[i] = GameSceneBlock[i - 1];
                }
            }
        }
        Score += cnt * 100;

    }
    bool isBlockCollide()
    {
        for(int BlockStartRow = 0, SceneStartRow = y; BlockStartRow < sc_BlockRow && SceneStartRow < sc_ArrRow && SceneStartRow < y + 4; BlockStartRow++, SceneStartRow++)
        {
            int rowValue = (block[BlockStartRow] << x) & GameSceneBlock[SceneStartRow];
            if(rowValue != 0)
            {
                return true;
            }
        }
        return false;
    }
    bool AutoMoveDown()
    {
        y++;
        if(isBlockCollide())
        {
            y--;
            return false;
        }
        return true;
    }
    void MoveLeft()
    {
        x++;
        if(isBlockCollide())
        {
            x--;
        }
    }
    void MoveRight()
    {
        x--;
        if(isBlockCollide())
        {
            x++;
        }
    }
    void BlockRotate()
    {
        int newState = (BlockState + 1) % 4;

        for(int i = 0; i < sc_BlockRow; i++)
        {
            block[i] = Tetrominos::Tetromino[BlockType][newState][i];
        }

        if(isBlockCollide())
        {
            for(int i = 0; i < sc_BlockRow; i++)
            {
                block[i] = Tetrominos::Tetromino[BlockType][BlockState][i];
            }
        }
        else
        {
            BlockState = newState;
        }
    }
    bool isGameOver()
    {
        //判定游戏结束
        if(GameSceneBlock[3] != 0b1110000000000111)
        {
            clearBlock();
            return true;
        }
        return false;
    }
    void clearBlock()
    {
        for(int i = 0; i < sc_BlockRow; i++)
        {
            block[i] = 0b0000;
        }
    }


};

#endif // BLOCK_H
