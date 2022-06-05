#pragma once
#include <fstream>
#include <vector>

namespace dae
{
	void BinToCharVec(std::vector<char>& output, const std::string& filePath)
	{
		if (std::ifstream input{ filePath, std::ios::binary }; input.is_open())
		{
			while (!input.eof())
			{
				char outputChar{};
				input.read(&outputChar, sizeof(outputChar));
				output.emplace_back(outputChar);
			}
			output.pop_back();
			input.close();
		}
	}
}