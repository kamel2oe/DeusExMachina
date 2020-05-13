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
                players.empty();

                for (int i = 0; i < 9; i++)
                {
                    uint32_t player_address = (table_client_data + 0xC08) + (player_size * i);

                    Player player(mem, player_address);

                    if (!player.IsValid())
                        continue;

                    if (player.IsOut())
                        continue;

                    players.push_back(player);

                    printf("Seat %i: %s (%i in chips)\n", i + 1, player.name, player.chip_size / 100);
                }

                printf("#%i is the button\n", button_id);
            }

            if (cached_turn_id != turn_id && turn_id != -1)
            {
                cached_turn_id = turn_id;

                printf("#%i turn\n", turn_id);
            }

            if (cached_cards_on_display_count != cards_on_display_count)
            {
                cached_cards_on_display_count = cards_on_display_count;

                switch (cards_on_display_count) {
                case 0: printf("*** HOLE CARDS ***\n"); break;
                case 3: printf("*** FLOP ***\n"); break;
                case 4: printf("*** TURN ***\n"); break;
                case 5: printf("*** RIVER ***\n"); break;
                }
            }
        }
    }
};