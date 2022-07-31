#include "LevelFromFile.h"
#include <fstream>
#include <sstream>
#include <assert.h>

void LevelFromFile::ReadFile(std::string filePath, std::vector<bool> level)
{
    level.clear();

    std::ifstream inFile{};
    inFile.open(filePath);

    int number{};
    std::string line{};

    assert(inFile.is_open() && "Could not open file");

    while (!inFile.eof())
    {
        std::getline(inFile, line);
        std::stringstream ss(line);
        while (ss >> number)
        {
            level.emplace_back(number);
            if (ss.peek() == ',')
            {
                ss.ignore();
            }
        }
    }
    inFile.close();

}
