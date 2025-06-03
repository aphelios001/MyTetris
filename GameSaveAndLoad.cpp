#include <QDebug>
#include "GameSaveAndLoad.h"

GameSaveAndLoad::GameSaveAndLoad() {}

bool GameSaveAndLoad::saveGame(const QString &filename, const Block &block, int score, int speed)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "无法写入，请检查文件权限: " << filename;
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    //文件标识设置：
    out << quint32(0x54455453); //TETS   54-T 45-E 54-T 53-S
    out << quint16(0x0100); //版本号100

    out << block << score << speed;
    file.close();

    return true;
}

bool GameSaveAndLoad::loadGame(const QString &filename, Block &block, int &score, int &speed)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "无法读入，请检查文件权限：" << filename;
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    quint32 magic;
    quint16 version;

    in >> magic >> version;

    if(magic != 0x54455453 || version != 0x100)
    {
        qWarning() << "无效的文件格式，请检查文件！";
        file.close();
        return false;
    }

    in >> block >> score >> speed;

    file.close();
    return true;
}
