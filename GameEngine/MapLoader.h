#pragma once
#include <iostream>
#include <vector>

class MapLoader
{
public:
	static void LoadMap(std::string MapName);

	static std::vector<std::vector<char>> MapData;
};

