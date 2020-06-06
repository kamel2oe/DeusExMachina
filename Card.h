#pragma once
#include <cstdint>

class Card
{
public:
    uint32_t card_data;
    int number;
    int type;

    Card(uint32_t card_data);
    ~Card();

    void Read();
};