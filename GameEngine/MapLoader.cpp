#include "MapLoader.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::vector<std::vector<char>> MapLoader::MapData;

void MapLoader::LoadMap(std::string MapName)
{
	MapLoader::MapData.clear();

	std::string line;
	std::ifstream fstream(MapName);
	int tileCode;

	if (fstream)
	{
		while (std::getline(fstream, line)) // Read each line from level file
		{
			std::istringstream sstream(line);
			std::vector<char> row;
			for (int i = 0; i < line.size(); i++)
			{
				row.push_back(line[i]);
			}
			MapData.push_back(row);
		}
	}
	
}
