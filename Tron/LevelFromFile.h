#pragma once
#include <string>
#include <vector>

class LevelFromFile final
{
	LevelFromFile() = default;
	~LevelFromFile() = default;

	LevelFromFile(const LevelFromFile& other) = delete;
	LevelFromFile(LevelFromFile&& other) = delete;
	LevelFromFile& operator=(const LevelFromFile& other) = delete;
	LevelFromFile& operator=(LevelFromFile&& other) = delete;

	void ReadFile(std::string filePath, std::vector<bool> level);
};

