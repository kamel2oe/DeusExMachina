#pragma once

#include "resource.h"
#include "images.h"
#include "fonts.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <iostream>

#pragma comment(lib, "d3d9.lib")

#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")

// Simple helper function to load an image into a DX9 texture with common settings
bool LoadTextureFromFile(LPDIRECT3DDEVICE9 device, const unsigned char* data, int size, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
    // Load texture from disk
    PDIRECT3DTEXTURE9 texture;
    //HRESULT hr = D3DXCreateTextureFromFileA(device, filename, &texture);
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

void DrawImageFromMemory(LPDIRECT3DDEVICE9 device, const unsigned char* data, int size, int width, int height)
{
    PDIRECT3DTEXTURE9 my_texture = NULL;
    int my_image_width = 0;
    int my_image_height = 0;
    bool ret = LoadTextureFromFile(device, data, size, &my_texture, &my_image_width, &my_image_height);
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

const char* pp_type(int type)
{
    switch (type)
    {
    case 99:  return "Clubs";
    case 100: return "Diamonds";
    case 104: return "Hearts";
    case 115: return "Spades";
    default:  return "";
    }
}

const char* pp_number(int number)
{
    switch (number)
    {
    case 1:  return "One";
    case 2:  return "Two";
    case 3:  return "Three";
    case 4:  return "Four";
    case 5:  return "Five";
    case 6:  return "Six";
    case 7:  return "Seven";
    case 8:  return "Eight";
    case 9:  return "Nine";
    case 10: return "Ten";
    case 11: return "Jack";
    case 12: return "Queen";
    case 13: return "King";
    case 14: return "Ace";
    default: return "";
    }
}

void DrawCard(LPDIRECT3DDEVICE9 device, int number, int type)
{
    switch (number)
    {

    case 2: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, two_clubs_png, sizeof(two_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, two_diamonds_png, sizeof(two_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, two_hearts_png, sizeof(two_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, two_spades_png, sizeof(two_spades_png), 134 / 2, 186 / 2);
    };

    case 3: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, three_clubs_png, sizeof(three_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, three_diamonds_png, sizeof(three_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, three_hearts_png, sizeof(three_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, three_spades_png, sizeof(three_spades_png), 134 / 2, 186 / 2);
    };

    case 4: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, four_clubs_png, sizeof(four_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, four_diamonds_png, sizeof(four_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, four_hearts_png, sizeof(four_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, four_spades_png, sizeof(four_spades_png), 134 / 2, 186 / 2);
    };

    case 5: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, five_clubs_png, sizeof(five_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, five_diamonds_png, sizeof(five_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, five_hearts_png, sizeof(five_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, five_spades_png, sizeof(five_spades_png), 134 / 2, 186 / 2);
    };

    case 6: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, six_clubs_png, sizeof(six_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, six_diamonds_png, sizeof(six_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, six_hearts_png, sizeof(six_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, six_spades_png, sizeof(six_spades_png), 134 / 2, 186 / 2);
    };

    case 7: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, seven_clubs_png, sizeof(seven_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, seven_diamonds_png, sizeof(seven_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, seven_hearts_png, sizeof(seven_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, seven_spades_png, sizeof(seven_spades_png), 134 / 2, 186 / 2);
    };

    case 8: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, eight_clubs_png, sizeof(eight_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, eight_diamonds_png, sizeof(eight_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, eight_hearts_png, sizeof(eight_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, eight_spades_png, sizeof(eight_spades_png), 134 / 2, 186 / 2);
    };

    case 9: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, nine_clubs_png, sizeof(nine_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, nine_diamonds_png, sizeof(nine_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, nine_hearts_png, sizeof(nine_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, nine_spades_png, sizeof(nine_spades_png), 134 / 2, 186 / 2);
    };

    case 10: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, ten_clubs_png, sizeof(ten_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, ten_diamonds_png, sizeof(ten_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, ten_hearts_png, sizeof(ten_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, ten_spades_png, sizeof(ten_spades_png), 134 / 2, 186 / 2);
    };

    case 11: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, jack_clubs_png, sizeof(jack_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, jack_diamonds_png, sizeof(jack_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, jack_hearts_png, sizeof(jack_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, jack_spades_png, sizeof(jack_spades_png), 134 / 2, 186 / 2);
    };

    case 12: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, queen_clubs_png, sizeof(queen_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, queen_diamonds_png, sizeof(queen_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, queen_hearts_png, sizeof(queen_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, queen_spades_png, sizeof(queen_spades_png), 134 / 2, 186 / 2);
    };

    case 13: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, king_clubs_png, sizeof(king_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, king_diamonds_png, sizeof(king_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, king_hearts_png, sizeof(king_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, king_spades_png, sizeof(king_spades_png), 134 / 2, 186 / 2);
    };

    case 14: switch (type)
    {
    case 99:  return DrawImageFromMemory(device, ace_clubs_png, sizeof(ace_clubs_png), 134 / 2, 186 / 2);
    case 100: return DrawImageFromMemory(device, ace_diamonds_png, sizeof(ace_diamonds_png), 134 / 2, 186 / 2);
    case 104: return DrawImageFromMemory(device, ace_hearts_png, sizeof(ace_hearts_png), 134 / 2, 186 / 2);
    case 115: return DrawImageFromMemory(device, ace_spades_png, sizeof(ace_spades_png), 134 / 2, 186 / 2);
    };

    default: return DrawImageFromMemory(device, blank_png, sizeof(blank_png), 134 / 2, 186 / 2);
    }
}