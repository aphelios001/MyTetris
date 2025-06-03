#ifndef GAMESAVEANDLOAD_H
#define GAMESAVEANDLOAD_H

#include <QFile>
#include <QDataStream>
#include "Block.h"

class GameSaveAndLoad
{
public:
    GameSaveAndLoad();
    static bool saveGame(const QString &filename, const Block &block, int score, int speed);
    static bool loadGame(const QString &filename, Block &block, int &score, int &speed);
};

#endif // GAMESAVEANDLOAD_H
