#pragma once
#include <fstream>
#include <vector>

namespace dae
{
	void BinToCharVec(std::vector<char>& output, const std::string& filePath);
}