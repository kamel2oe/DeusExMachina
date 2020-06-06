#include "Memory.h"
#include "Table.h"

Table::Table(uint32_t table_data)
{
    this->table_data = table_data;
}

Table::~Table()
{
}

void Table::Read()
{
    cards.clear();
    players.clear();

    uint32_t table_client_data = mem->Read<uint32_t>(table_data - 0x14);

    uint32_t table_name_ptr = mem->Read<uint32_t>(table_client_data + 0xC4);
    mem->ReadBuffer(table_name_ptr, &name, sizeof(name));

    cards_on_display_count = mem->Read<uint32_t>(table_client_data + 0xB50);
    pot_size = mem->Read<uint32_t>(table_client_data + 0x234);

    button_id = mem->Read<uint32_t>(table_data + 0xF20);
    turn_id = mem->Read<uint32_t>(table_data + 0xF24);

    ante = mem->Read<uint32_t>(table_client_data + 0x298);
    small_blind = mem->Read<uint32_t>(table_client_data + 0xEC);
    big_blind = mem->Read<uint32_t>(table_client_data + 0xF0);

    for (int i = 0; i < 5; i++) {
        uint32_t card_data = (table_client_data + 0xB5C) + (0x8 * i);

        Card card(card_data);
        card.Read();

        cards.push_back(card);
    }

    uint32_t player_size = 0x1F8;

    for (int i = 0; i < 9; i++)
    {
        uint32_t player_data = (table_client_data + 0xC08) + (player_size * i);

        Player player(player_data);
        player.Read();

        if (player.IsValid())
        {
            players.push_back(player);
        }
    }
}
