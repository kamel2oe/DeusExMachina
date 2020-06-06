#include "Game.h"
#include "Memory.h"
#include "Table.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Read()
{
    std::vector<uintptr_t> table_addresses = mem->FindReferences("\x10\x2F\x90\x01\x01", "xxxxx", 100);

    for (auto& table_address : table_addresses) 
    {
        Table table(table_address);
        table.Read();

        std::string table_name = table.name;

        auto it = std::find_if(tables.begin(), tables.end(), [&table_name](const Table& obj) {return obj.name == table_name; });

        if (it != tables.end())
        {
            break;
        }
        else {
            tables.push_back(table);
        }
    }
}
