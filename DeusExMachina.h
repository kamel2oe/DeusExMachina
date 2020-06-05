#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include "fonts.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

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