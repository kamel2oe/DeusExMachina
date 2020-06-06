#include "Card.h"
#include "Memory.h"

Card::Card(uint32_t card_data)
{
	this->card_data = card_data;
}

Card::~Card()
{
}

void Card::Read()
{
	number = mem->Read<int>(card_data);
	type = mem->Read<int>(card_data + 0x4);
}
