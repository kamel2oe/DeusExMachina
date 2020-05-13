#pragma once

#include "resource.h"
#include "images.h"
#include "fonts.h"
#include "Memory.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <iostream>
#include <sstream>
#include <string>

#pragma comment(lib, "d3d9.lib")

#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")

PDIRECT3DTEXTURE9 two_clubs_tex = NULL;
PDIRECT3DTEXTURE9 two_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 two_hearts_tex = NULL;
PDIRECT3DTEXTURE9 two_spades_tex = NULL;
PDIRECT3DTEXTURE9 three_clubs_tex = NULL;
PDIRECT3DTEXTURE9 three_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 three_hearts_tex = NULL;
PDIRECT3DTEXTURE9 three_spades_tex = NULL;
PDIRECT3DTEXTURE9 four_clubs_tex = NULL;
PDIRECT3DTEXTURE9 four_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 four_hearts_tex = NULL;
PDIRECT3DTEXTURE9 four_spades_tex = NULL;
PDIRECT3DTEXTURE9 five_clubs_tex = NULL;
PDIRECT3DTEXTURE9 five_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 five_hearts_tex = NULL;
PDIRECT3DTEXTURE9 five_spades_tex = NULL;
PDIRECT3DTEXTURE9 six_clubs_tex = NULL;
PDIRECT3DTEXTURE9 six_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 six_hearts_tex = NULL;
PDIRECT3DTEXTURE9 six_spades_tex = NULL;
PDIRECT3DTEXTURE9 seven_clubs_tex = NULL;
PDIRECT3DTEXTURE9 seven_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 seven_hearts_tex = NULL;
PDIRECT3DTEXTURE9 seven_spades_tex = NULL;
PDIRECT3DTEXTURE9 eight_clubs_tex = NULL;
PDIRECT3DTEXTURE9 eight_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 eight_hearts_tex = NULL;
PDIRECT3DTEXTURE9 eight_spades_tex = NULL;
PDIRECT3DTEXTURE9 nine_clubs_tex = NULL;
PDIRECT3DTEXTURE9 nine_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 nine_hearts_tex = NULL;
PDIRECT3DTEXTURE9 nine_spades_tex = NULL;
PDIRECT3DTEXTURE9 ten_clubs_tex = NULL;
PDIRECT3DTEXTURE9 ten_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 ten_hearts_tex = NULL;
PDIRECT3DTEXTURE9 ten_spades_tex = NULL;
PDIRECT3DTEXTURE9 jack_clubs_tex = NULL;
PDIRECT3DTEXTURE9 jack_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 jack_hearts_tex = NULL;
PDIRECT3DTEXTURE9 jack_spades_tex = NULL;
PDIRECT3DTEXTURE9 queen_clubs_tex = NULL;
PDIRECT3DTEXTURE9 queen_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 queen_hearts_tex = NULL;
PDIRECT3DTEXTURE9 queen_spades_tex = NULL;
PDIRECT3DTEXTURE9 king_clubs_tex = NULL;
PDIRECT3DTEXTURE9 king_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 king_hearts_tex = NULL;
PDIRECT3DTEXTURE9 king_spades_tex = NULL;
PDIRECT3DTEXTURE9 ace_clubs_tex = NULL;
PDIRECT3DTEXTURE9 ace_diamonds_tex = NULL;
PDIRECT3DTEXTURE9 ace_hearts_tex = NULL;
PDIRECT3DTEXTURE9 ace_spades_tex = NULL;
PDIRECT3DTEXTURE9 blank_tex = NULL;
PDIRECT3DTEXTURE9 back_tex = NULL;

// Simple helper function to load an image into a DX9 texture with common settings
bool LoadTextureFromMemory(LPDIRECT3DDEVICE9 device, const unsigned char* data, int size, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
    PDIRECT3DTEXTURE9 texture;
    HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(device, data, size, 300, 100, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texture);
    if (hr != S_OK)
        return false;

    // Retrieve description of the texture surface so we can access its size
    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);
    *out_texture = texture;
    *out_width = (int)my_image_desc.Width;
    *out_height = (int)my_image_desc.Height;
    return true;
}

PDIRECT3DTEXTURE9 LoadImageFromMemory(LPDIRECT3DDEVICE9 device, const unsigned char* data, int size)
{
    PDIRECT3DTEXTURE9 my_texture;
    int my_image_width = 0;
    int my_image_height = 0;
    bool ret = LoadTextureFromMemory(device, data, size, &my_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);

    return my_texture;
}

void DrawImageFromMemory(LPDIRECT3DDEVICE9 device, PDIRECT3DTEXTURE9 my_texture, const unsigned char* data, int size, int width, int height)
{
    int my_image_width = 0;
    int my_image_height = 0;
    bool ret = LoadTextureFromMemory(device, data, size, &my_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);

    ImGui::Image((void*)my_texture, ImVec2(width, height));
}

static inline ImVec2 operator*(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }

#ifdef _DEBUG  
#define D(x) (x)
#else 
#define D(x) do{}while(0)
#endif

FILE* fOut;

void OpenConsole()
{
    AllocConsole();
    freopen_s(&fOut, "CONOUT$", "w", stdout);
}

void RemoveConsole()
{
    fclose(fOut);
    FreeConsole();
}

enum CardType : int
{
    Clubs = 99,
    Diamonds = 100,
    Hearts = 104,
    Spades = 115
};

std::string pp_type(int type)
{
    switch (type)
    {
    case CardType::Clubs:    return "c";
    case CardType::Diamonds: return "d";
    case CardType::Hearts:   return "h";
    case CardType::Spades:   return "s";
    }

    return "";
}

enum CardNumber : int
{
    Two = 2,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

std::string pp_number(int number)
{
    switch (number)
    {
    case CardNumber::Two:   return "2";
    case CardNumber::Three: return "3";
    case CardNumber::Four:  return "4";
    case CardNumber::Five:  return "5";
    case CardNumber::Six:   return "6";
    case CardNumber::Seven: return "7";
    case CardNumber::Eight: return "8";
    case CardNumber::Nine:  return "9";
    case CardNumber::Ten:   return "t";
    case CardNumber::Jack:  return "j";
    case CardNumber::Queen: return "q";
    case CardNumber::King:  return "k";
    case CardNumber::Ace:   return "a";
    }

    return "";
}

std::string pp_card(int number, int type)
{
    return pp_number(number) + pp_type(type);
}

void LoadImages(LPDIRECT3DDEVICE9 device)
{
    two_clubs_tex = LoadImageFromMemory(device, two_clubs_png, sizeof(two_clubs_png));
    two_diamonds_tex = LoadImageFromMemory(device, two_diamonds_png, sizeof(two_diamonds_png));
    two_hearts_tex = LoadImageFromMemory(device, two_hearts_png, sizeof(two_hearts_png));
    two_spades_tex = LoadImageFromMemory(device, two_spades_png, sizeof(two_spades_png));
    three_clubs_tex = LoadImageFromMemory(device, three_clubs_png, sizeof(three_clubs_png));
    three_diamonds_tex = LoadImageFromMemory(device, three_diamonds_png, sizeof(three_diamonds_png));
    three_hearts_tex = LoadImageFromMemory(device, three_hearts_png, sizeof(three_hearts_png));
    three_spades_tex = LoadImageFromMemory(device, three_spades_png, sizeof(three_spades_png));
    four_clubs_tex = LoadImageFromMemory(device, four_clubs_png, sizeof(four_clubs_png));
    four_diamonds_tex = LoadImageFromMemory(device, four_diamonds_png, sizeof(four_diamonds_png));
    four_hearts_tex = LoadImageFromMemory(device, four_hearts_png, sizeof(four_hearts_png));
    four_spades_tex = LoadImageFromMemory(device, four_spades_png, sizeof(four_spades_png));
    five_clubs_tex = LoadImageFromMemory(device, five_clubs_png, sizeof(five_clubs_png));
    five_diamonds_tex = LoadImageFromMemory(device, five_diamonds_png, sizeof(five_diamonds_png));
    five_hearts_tex = LoadImageFromMemory(device, five_hearts_png, sizeof(five_hearts_png));
    five_spades_tex = LoadImageFromMemory(device, five_spades_png, sizeof(five_spades_png));
    six_clubs_tex = LoadImageFromMemory(device, six_clubs_png, sizeof(six_clubs_png));
    six_diamonds_tex = LoadImageFromMemory(device, six_diamonds_png, sizeof(six_diamonds_png));
    six_hearts_tex = LoadImageFromMemory(device, six_hearts_png, sizeof(six_hearts_png));
    six_spades_tex = LoadImageFromMemory(device, six_spades_png, sizeof(six_spades_png));
    seven_clubs_tex = LoadImageFromMemory(device, seven_clubs_png, sizeof(seven_clubs_png));
    seven_diamonds_tex = LoadImageFromMemory(device, seven_diamonds_png, sizeof(seven_diamonds_png));
    seven_hearts_tex = LoadImageFromMemory(device, seven_hearts_png, sizeof(seven_hearts_png));
    seven_spades_tex = LoadImageFromMemory(device, seven_spades_png, sizeof(seven_spades_png));
    eight_clubs_tex = LoadImageFromMemory(device, eight_clubs_png, sizeof(eight_clubs_png));
    eight_diamonds_tex = LoadImageFromMemory(device, eight_diamonds_png, sizeof(eight_diamonds_png));
    eight_hearts_tex = LoadImageFromMemory(device, eight_hearts_png, sizeof(eight_hearts_png));
    eight_spades_tex = LoadImageFromMemory(device, eight_spades_png, sizeof(eight_spades_png));
    nine_clubs_tex = LoadImageFromMemory(device, nine_clubs_png, sizeof(nine_clubs_png));
    nine_diamonds_tex = LoadImageFromMemory(device, nine_diamonds_png, sizeof(nine_diamonds_png));
    nine_hearts_tex = LoadImageFromMemory(device, nine_hearts_png, sizeof(nine_hearts_png));
    nine_spades_tex = LoadImageFromMemory(device, nine_spades_png, sizeof(nine_spades_png));
    ten_clubs_tex = LoadImageFromMemory(device, ten_clubs_png, sizeof(ten_clubs_png));
    ten_diamonds_tex = LoadImageFromMemory(device, ten_diamonds_png, sizeof(ten_diamonds_png));
    ten_hearts_tex = LoadImageFromMemory(device, ten_hearts_png, sizeof(ten_hearts_png));
    ten_spades_tex = LoadImageFromMemory(device, ten_spades_png, sizeof(ten_spades_png));
    jack_clubs_tex = LoadImageFromMemory(device, jack_clubs_png, sizeof(jack_clubs_png));
    jack_diamonds_tex = LoadImageFromMemory(device, jack_diamonds_png, sizeof(jack_diamonds_png));
    jack_hearts_tex = LoadImageFromMemory(device, jack_hearts_png, sizeof(jack_hearts_png));
    jack_spades_tex = LoadImageFromMemory(device, jack_spades_png, sizeof(jack_spades_png));
    queen_clubs_tex = LoadImageFromMemory(device, queen_clubs_png, sizeof(queen_clubs_png));
    queen_diamonds_tex = LoadImageFromMemory(device, queen_diamonds_png, sizeof(queen_diamonds_png));
    queen_hearts_tex = LoadImageFromMemory(device, queen_hearts_png, sizeof(queen_hearts_png));
    queen_spades_tex = LoadImageFromMemory(device, queen_spades_png, sizeof(queen_spades_png));
    king_clubs_tex = LoadImageFromMemory(device, king_clubs_png, sizeof(king_clubs_png));
    king_diamonds_tex = LoadImageFromMemory(device, king_diamonds_png, sizeof(king_diamonds_png));
    king_hearts_tex = LoadImageFromMemory(device, king_hearts_png, sizeof(king_hearts_png));
    king_spades_tex = LoadImageFromMemory(device, king_spades_png, sizeof(king_spades_png));
    ace_clubs_tex = LoadImageFromMemory(device, ace_clubs_png, sizeof(ace_clubs_png));
    ace_diamonds_tex = LoadImageFromMemory(device, ace_diamonds_png, sizeof(ace_diamonds_png));
    ace_hearts_tex = LoadImageFromMemory(device, ace_hearts_png, sizeof(ace_hearts_png));
    ace_spades_tex = LoadImageFromMemory(device, ace_spades_png, sizeof(ace_spades_png));
    blank_tex = LoadImageFromMemory(device, blank_png, sizeof(blank_png));
    back_tex = LoadImageFromMemory(device, back_png, sizeof(back_png));
}

PDIRECT3DTEXTURE9 GetCardTexture(LPDIRECT3DDEVICE9 device, int number, int type)
{
    switch (number)
    {

    case 2: switch (type)
    {
    case 99:  return two_clubs_tex;
    case 100: return two_diamonds_tex;
    case 104: return two_hearts_tex;
    case 115: return two_spades_tex;
    };

    case 3: switch (type)
    {
    case 99:  return three_clubs_tex;
    case 100: return three_diamonds_tex;
    case 104: return three_hearts_tex;
    case 115: return three_spades_tex;
    };

    case 4: switch (type)
    {
    case 99:  return four_clubs_tex;
    case 100: return four_diamonds_tex;
    case 104: return four_hearts_tex;
    case 115: return four_spades_tex;
    };

    case 5: switch (type)
    {
    case 99:  return five_clubs_tex;
    case 100: return five_diamonds_tex;
    case 104: return five_hearts_tex;
    case 115: return five_spades_tex;
    };

    case 6: switch (type)
    {
    case 99:  return six_clubs_tex;
    case 100: return six_diamonds_tex;
    case 104: return six_hearts_tex;
    case 115: return six_spades_tex;
    };

    case 7: switch (type)
    {
    case 99:  return seven_clubs_tex;
    case 100: return seven_diamonds_tex;
    case 104: return seven_hearts_tex;
    case 115: return seven_spades_tex;
    };

    case 8: switch (type)
    {
    case 99:  return eight_clubs_tex;
    case 100: return eight_diamonds_tex;
    case 104: return eight_hearts_tex;
    case 115: return eight_spades_tex;
    };

    case 9: switch (type)
    {
    case 99:  return nine_clubs_tex;
    case 100: return nine_diamonds_tex;
    case 104: return nine_hearts_tex;
    case 115: return nine_spades_tex;
    };

    case 10: switch (type)
    {
    case 99:  return ten_clubs_tex;
    case 100: return ten_diamonds_tex;
    case 104: return ten_hearts_tex;
    case 115: return ten_spades_tex;
    };

    case 11: switch (type)
    {
    case 99:  return jack_clubs_tex;
    case 100: return jack_diamonds_tex;
    case 104: return jack_hearts_tex;
    case 115: return jack_spades_tex;
    };

    case 12: switch (type)
    {
    case 99:  return queen_clubs_tex;
    case 100: return queen_diamonds_tex;
    case 104: return queen_hearts_tex;
    case 115: return queen_spades_tex;
    };

    case 13: switch (type)
    {
    case 99:  return king_clubs_tex;
    case 100: return king_diamonds_tex;
    case 104: return king_hearts_tex;
    case 115: return king_spades_tex;
    };

    case 14: switch (type)
    {
    case 99:  return ace_clubs_tex;
    case 100: return ace_diamonds_tex;
    case 104: return ace_hearts_tex;
    case 115: return ace_spades_tex;
    };

    default: return back_tex;
    }
}