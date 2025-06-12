#include <QDebug>
#include "GameSaveAndLoad.h"

GameSaveAndLoad::GameSaveAndLoad() {}

bool GameSaveAndLoad::saveGame(const QString &filename, const Block &block, int score, int speed)
{
    // 打开指定文件，以只写模式
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "无法写入，请检查文件权限: " << filename;
        return false;
    }

    // 创建数据流对象，并设置Qt版本兼容性
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    // 写入文件标识符（Magic Number）和版本号，用于识别和验证文件格式
    out << quint32(0x54455453); // ASCII码 'TETS'，表示这是本游戏的存档文件
    out << quint16(0x0100);     // 版本号 1.00

    // 将游戏核心数据序列化写入文件
    out << block << score << speed;

    // 关闭文件
    file.close();

    return true; // 返回保存成功
}

// 注意score 和 speed传的是引用，用于返回score 和 speed
bool GameSaveAndLoad::loadGame(const QString &filename, Block &block, int &score, int &speed)
{
    // 打开指定文件，以只读模式
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "无法读入，请检查文件权限：" << filename;
        return false;
    }

    // 创建数据流对象，并设置Qt版本兼容性
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    // 读取文件标识符和版本号，用于验证文件是否合法
    quint32 magic;
    quint16 version;
    in >> magic >> version;

    // 校验文件标识和版本是否匹配预期值
    if(magic != 0x54455453 || version != 0x100)
    {
        qWarning() << "无效的文件格式，请检查文件！";
        file.close();
        return false;
    }

    // 反序列化读取游戏数据：Block对象、分数、速度
    in >> block >> score >> speed;

    // 关闭文件
    file.close();

    return true; // 返回加载成功
}
