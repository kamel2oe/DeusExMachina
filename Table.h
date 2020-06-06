#pragma once
#include <cstdint>
#include <vector>
#include "Player.h"
#include "Card.h"

class Table
{
public:
	uint32_t table_data;
	int round_number;
	char name[50];

	uint32_t cards_on_display_count;
	uint32_t pot_size;

	uint32_t button_id;
	uint32_t turn_id;

	uint32_t ante;
	uint32_t small_blind;
	uint32_t big_blind;

	int play_mode;

	std::vector<Player> players;
	std::vector<Card> cards;

	Table(uint32_t table_data);
	~Table();

	void Read();
};