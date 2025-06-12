#include "block.h"

// 重载 << 运算符，用于保存 Block 对象的状态
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

// 重载 >> 运算符，用于读取 Block 对象的状态。
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
    // 遍历场景中的每一行（共24行）
    for(int i = 0; i < sc_ArrRow; i++)
    {
        // 将当前行设置为默认场景数据
        GameSceneBlock[i] = DefaultSceneBlock[i];
    }
}

void Block::GetNextBlock()
{
    // 使用 Qt 提供的随机数生成器，生成 0~6 的随机整数，表示七种方块之一
    int randomTypeInt = QRandomGenerator::global()->bounded(7);

    // 记录下一个方块的类型
    NextBlockType = randomTypeInt;

    // 遍历方块的四行数据
    for(int i = 0; i < sc_BlockRow; i++)
    {
        // 从 Tetrominos::Tetromino[类型][方向][行] 中取出对应数据，保存到 next_block
        next_block[i] = Tetrominos::Tetromino[randomTypeInt][BlockState][i];
    }
}

void Block::CreateNewBlock()
{
    // 设置新方块的初始水平位置（居中）
    x = 5;
    // 设置新方块的起始垂直位置（顶部缓冲区下方）
    y = 0;
    // 方块初始旋转状态设为 0（即第一个方向）
    BlockState = 0;
    // 当前方块类型设为下一个方块的类型
    CurrentBlockType = NextBlockType;

    // 将 next_block 中的方块数据复制到当前方块 block 数组中
    for(int i = 0; i < sc_BlockRow; i++)
    {
        block[i] = next_block[i];
    }

    // 更新下一个方块的数据（提前准备下一个方块）
    GetNextBlock();
}

void Block::MergeSceneAndBlock(int &Score)
{
    //将当前方块合并到游戏场景，并检查是否有完整的行可以消除

    // 遍历方块的每一行（从y坐标开始，最多4行）
    for(int SceneRow = y, BlockRow = 0; SceneRow < sc_ArrRow && SceneRow < y + 4 && BlockRow < sc_BlockRow; SceneRow++, BlockRow++)
    {
        // 将方块当前行左移x位（调整水平位置），然后与场景行按位或合并
        int rowValue = (block[BlockRow] << x) | GameSceneBlock[SceneRow];

        // 更新游戏场景的行数据
        GameSceneBlock[SceneRow] = rowValue;
    }

    // 记录消除的行数
    int cnt = 0;

    /*
    // 遍历游戏场景的每一行（跳过顶部3行缓冲区）
    for(int Row = 0; Row < sc_GameRow; Row++)
    {
        // 检查当前行是否被完全填满（16位全1）
        if(GameSceneBlock[Row + 3] == sc_FullRow)
        {
            cnt++;// 增加消除计数
            // 从当前行开始，逐行下移（上面的行覆盖当前行）
            // 这里可优化
            for(int i = Row + 3; i >= 3; i--)
            {
                GameSceneBlock[i] = GameSceneBlock[i - 1];
            }
        }
    }
    */

    //优化后的版本（利用了快慢双指针）：
    int writeRow = sc_GameRow + 2; // 第22行开始 到 第3行结束（实际游戏场景）

    for(int readRow = sc_GameRow + 2; readRow >= 3; readRow--)
    {
        // 检查当前行是否被完全填满（需要消除）
        if(GameSceneBlock[readRow] == sc_FullRow)
        {
            cnt++;// 统计消除的行数
        }
        else
        {
            // 如果当前行不需要消除，则将其数据写入 writeRow 位置，非满行列下移
            GameSceneBlock[writeRow] = GameSceneBlock[readRow];
            // 写入指针上移，准备写入下一行
            writeRow--;
        }
    }

    // 处理剩余未覆盖的行（顶部空行补充）
    while(writeRow >= 3)
    {
        // 将剩余的行清零（补充空行（空行不是全0））
        GameSceneBlock[writeRow] = 0b1110000000000111;
        // 继续上移写入指针，直到 writeRow < 3 为止
        writeRow--;
    }

    // 更新分数（每消除一行加100分），可修改。
    Score += cnt * 100;
}

bool Block::isBlockCollide()
{
    // 循环遍历方块的每一行
    // BlockRow: 方块内部的行索引 (0-3)
    // SceneRow: 游戏场景中的行索引 (从y坐标开始)

    for(int BlockRow = 0, SceneRow = y; BlockRow < sc_BlockRow && SceneRow < sc_ArrRow && SceneRow < y + 4; BlockRow++, SceneRow++)
    {
        // 将方块当前行左移x位，然后与游戏场景的对应行进行按位与操作
        int rowValue = (block[BlockRow] << x) & GameSceneBlock[SceneRow];

        // 如果结果不为0，表示有重叠（碰撞）
        if(rowValue != 0)
        {
            return true;// 发生碰撞
        }
    }

    return false;// 没有碰撞
}

//类似回溯
// 下落核心逻辑（其他移动类似）
bool Block::AutoMoveDown()
{
    y++; // 尝试移动
    if(isBlockCollide())
    {
        y--; // 碰撞则回退
        return false;
    }
    return true;
}

// 左移右移通过x增减实现
void Block::MoveLeft()
{
    x++; // 尝试移动
    if(isBlockCollide())
    {
        x--; // 碰撞则恢复
    }
}

void Block::MoveRight()
{
    x--; // 尝试移动
    if(isBlockCollide())
    {
        x++; // 碰撞则恢复
    }
}

void Block::BlockRotate()
{
    int newState = (BlockState + 1) % 4;
    // 先尝试加载新方向数据
    for(int i = 0; i < sc_BlockRow; i++)
    {
        block[i] = Tetrominos::Tetromino[CurrentBlockType][newState][i];
    }

    // 如碰撞，则回退
    if(isBlockCollide())
    {
        for(int i = 0; i < sc_BlockRow; i++)
        {
            block[i] = Tetrominos::Tetromino[CurrentBlockType][BlockState][i];
        }
    }
    // 成功则保留
    else
    {
        BlockState = newState;
    }
}

bool Block::isGameOver()
{
    // 检查最顶行是否有方块（正常应保持初始边界值）
    if(GameSceneBlock[3] != 0b1110000000000111)
    {
        clearBlock();
        return true;
    }
    return false;
}

void Block::clearBlock()
{
    // 清空当前方块
    for(int i = 0; i < sc_BlockRow; i++)
    {
        block[i] = 0b0000;
    }
}
