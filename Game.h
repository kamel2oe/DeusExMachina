#pragma once
#include <vector>
#include "Table.h"

class Game
{
public:
	Game();
	~Game();

	std::vector<Table> tables;

	void Read();
};