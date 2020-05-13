#include <cstdint>

class Card
{
    uintptr_t base;
    Memory* mem;
public:
    int number;
    int type;

    Card(Memory* memory, uintptr_t card_address)
    {
        base = card_address;
        Read();
    }

    void Read()
    {
        number = mem->Read<int>(base);
        type = mem->Read<int>(base + 0x4);
    }
};