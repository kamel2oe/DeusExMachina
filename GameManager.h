#include <cstdint>

class GameManager
{
    Memory* mem;
    uintptr_t table_manager;
public:
    uint32_t cached_cards_on_display_count = -1;
    uint32_t cached_button_id = -1;
    uint32_t cached_turn_id = -1;
    std::vector<Player> players;
    std::vector<Card> cards;
    uint32_t player_size = 0x1F8;

    GameManager(Memory* memory, uintptr_t table_manager_address)
    {
        mem = memory;
        table_manager = table_manager_address;
    }

    void Read()
    {
        uint32_t normal_tables = mem->Read<uint32_t>(table_manager + 0x14);

        int table_index = 0;

        uint32_t table = mem->Read<uint32_t>(normal_tables + (0x4 * table_index));
        uint32_t table_client_data = mem->Read<uint32_t>(table + 0x164);

        char table_name[15];
        uint32_t table_name_ptr = mem->Read<uint32_t>(table_client_data + 0xC4);
        mem->ReadBuffer(table_name_ptr, &table_name, sizeof(table_name));

        printf("Table '%s'\n", table_name);

        while (true) {
            uint32_t cards_on_display_count = mem->Read<uint32_t>(table_client_data + 0xB50);
            uint32_t button_id = mem->Read<uint32_t>(table + 0xF20);
            uint32_t turn_id = mem->Read<uint32_t>(table + 0xF24);

            if (cached_button_id != button_id)
            {
                cached_button_id = button_id;

                printf("NEW ROUND\n");
                players.clear();
                cards.clear();

                for (int i = 0; i < 9; i++)
                {
                    uint32_t player_address = (table_client_data + 0xC08) + (player_size * i);

                    Player player(mem, player_address, i);

                    if (!player.IsValid())
                        continue;

                    players.insert(players.begin() + i, player);

                    printf("Seat %i: %s (%i in chips) %s\n", i, player.name, player.chip_size, player.IsOut() ? "OUT" : "IN");
                }

                printf("#%i is the button\n", button_id);
            }

            if (cached_turn_id != turn_id && turn_id != -1)
            {
                uint32_t previous_turn_id = cached_turn_id;

                cached_turn_id = turn_id;

                if (previous_turn_id != -1)
                {
                    Player current_player = players.at(previous_turn_id);

                    current_player.Read();

                    if (current_player.IsOut())
                    {
                        printf("%s folds\n", current_player.name);
                    }
                    else
                    {
                        if (current_player.bet_amount > 0)
                        {
                            printf("%s bets %i\n", current_player.name, current_player.bet_amount);
                        }
                        else
                        {
                            printf("%s checks\n", current_player.name);
                        }
                    }
                }

                //Player player = players.at(turn_id);
                //printf("%s turn #%i\n", player.name, turn_id);
            }

            if (cached_cards_on_display_count != cards_on_display_count)
            {

                cached_cards_on_display_count = cards_on_display_count;

                switch (cards_on_display_count) {
                case 0: printf("*** HOLE CARDS ***\n"); break;
                case 3: printf("*** FLOP *** "); break;
                case 4: printf("*** TURN *** "); break;
                case 5: printf("*** RIVER *** "); break;
                }

                if (cards_on_display_count > 0)
                {
                    for (int i = 0; i < cards_on_display_count; i++) {
                        uint32_t card_address = (table_client_data + 0xB5C) + (0x8 * i);

                        Card card(mem, card_address);

                        printf("%s ", pp_card(card.number, card.type));
                    }

                    printf("\n");
                }
            }
        }
    }
};