#include "Player.h"
#include "Memory.h"

Player::Player(uint32_t player_data)
{
	this->player_data = player_data;
}

Player::~Player()
{
}

void Player::Read()
{
	mem->ReadBuffer(player_data + 0x8, &name, sizeof(name));
}

bool Player::IsValid()
{
	mem->ReadBuffer(player_data + 0x8, &name, sizeof(name));
	return strlen(name) != 0;
}
