#include <string>
#include <vector>
#include <fstream>

#ifndef FILEMANAGER_H
#define FILEMANAGER_H


class Game;


class FileManager {
    std::string filePath;

public:
    FileManager(const std::string& path);

    void saveToFile(Game& game);

    void loadFromFile(std::string playerName, Game& Game);
    std::vector<std::pair<std::wstring, int>> getAllPlayers();
    std::vector<std::string> getSavedPlayerNames();
};



#endif // _FILEMANAGER_H_

