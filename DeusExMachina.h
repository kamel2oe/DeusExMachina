#pragma once

#include "resource.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <iostream>

#pragma comment(lib, "d3d9.lib")

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