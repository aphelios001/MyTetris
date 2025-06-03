#include "block.h"

QDataStream& operator <<(QDataStream &out, const Block &block)
{
    out << block.x << block.y << block.CurrentBlockType << block.NextBlockType << block.BlockState;

    for(int i = 0; i < Block::sc_ArrRow; i++)
    {
        out << block.GameSceneBlock[i];
    }

    for(int i = 0; i < Block::sc_BlockRow; i++)
    {
        out << block.block[i];
    }

    for(int i = 0; i < Block::sc_BlockRow; i++)
    {
        out << block.next_block[i];
    }

    return out;
}

QDataStream& operator >>(QDataStream &in, Block &block)
{
    in >> block.x >> block.y >> block.CurrentBlockType >> block.NextBlockType >> block.BlockState;

    for(int i = 0; i < Block::sc_ArrRow; i++)
    {
        in >> block.GameSceneBlock[i];
    }

    for(int i = 0; i < Block::sc_BlockRow; i++)
    {
        in >> block.block[i];
    }

    for(int i = 0; i < Block::sc_BlockRow; i++)
    {
        in >> block.next_block[i];
    }

    return in;
}

int Block::getY()
{
    return y;
}

int Block::getX()
{
    return x;
}

void Block::InitScene()
{
    for(int i = 0; i < sc_ArrRow; i++)
    {
        GameSceneBlock[i] = DefaultSceneBlock[i];
    }
}

void Block::GetNextBlock()
{
    int randomTypeInt = QRandomGenerator::global()->bounded(7);
    NextBlockType = randomTypeInt;
    for(int i = 0; i < sc_BlockRow; i++)
    {
        next_block[i] = Tetrominos::Tetromino[randomTypeInt][BlockState][i];
    }
}

void Block::CreateNewBlock()
{
    x = 5;
    y = 0;
    BlockState = 0;
    CurrentBlockType = NextBlockType;

    for(int i = 0; i < sc_BlockRow; i++)
    {
        block[i] = next_block[i];
    }

    GetNextBlock();
}

void Block::MergeSceneAndBlock(int &Score)
{
    for(int SceneStartRow = y, BlockStartRow = 0; SceneStartRow < sc_ArrRow && SceneStartRow < y + 4 && BlockStartRow < sc_BlockRow; SceneStartRow++, BlockStartRow++)
    {
        int rowValue = (block[BlockStartRow] << x) | GameSceneBlock[SceneStartRow];
        GameSceneBlock[SceneStartRow] = rowValue;
    }

    int cnt = 0;
    for(int Row = 0; Row < sc_GameRow; Row++)
    {
        if(GameSceneBlock[Row + 3] == 0b1111111111111111)
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

bool Block::isBlockCollide()
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

bool Block::AutoMoveDown()
{
    y++;
    if(isBlockCollide())
    {
        y--;
        return false;
    }
    return true;
}

void Block::MoveLeft()
{
    x++;
    if(isBlockCollide())
    {
        x--;
    }
}

void Block::MoveRight()
{
    x--;
    if(isBlockCollide())
    {
        x++;
    }
}

void Block::BlockRotate()
{
    int newState = (BlockState + 1) % 4;

    for(int i = 0; i < sc_BlockRow; i++)
    {
        block[i] = Tetrominos::Tetromino[CurrentBlockType][newState][i];
    }

    if(isBlockCollide())
    {
        for(int i = 0; i < sc_BlockRow; i++)
        {
            block[i] = Tetrominos::Tetromino[CurrentBlockType][BlockState][i];
        }
    }
    else
    {
        BlockState = newState;
    }
}

bool Block::isGameOver()
{
    if(GameSceneBlock[3] != 0b1110000000000111)
    {
        clearBlock();
        return true;
    }
    return false;
}

void Block::clearBlock()
{
    for(int i = 0; i < sc_BlockRow; i++)
    {
        block[i] = 0b0000;
    }
}
