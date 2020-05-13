#include <cstdint>

class Player
{
    uintptr_t base;
    Memory* mem;
public:
    char name[15];
    int first_card_number;
    int first_card_type;
    int second_card_number;
    int second_card_type;
    int chip_size;
    int bet_amount;
    int bet_total;
    int state;

    Player(Memory* memory, uintptr_t player_address) {
        base = player_address;
        mem = memory;
        Read();
    }

    void Read()
    {
        mem->ReadBuffer(base + 0x8, &name, sizeof(name));

        first_card_number = mem->Read<int>(base + 0xF0);
        first_card_type = mem->Read<int>(base + 0xF4);

        second_card_number = mem->Read<int>(base + 0xF8);
        second_card_type = mem->Read<int>(base + 0xFC);

        chip_size = mem->Read<int>(base + 0xB0);
        bet_amount = mem->Read<int>(base + 0xB8);
        bet_total = mem->Read<int>(base + 0x88);
        state = mem->Read<int>(base + 0xD0);
    }

    bool IsValid()
    {
        mem->ReadBuffer(base + 0x8, &name, sizeof(name));
        return strlen(name) != 0;
    }

    // Folded / Sitting out / No Money Left
    bool IsOut()
    {
        return (state == 1 || state == 0);
    }
};