#include "Memory.h"
#include "Player.h"
#include "Card.h"
#include "images.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include <vector>

#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")
#pragma comment(lib, "d3d9.lib")

void Draw(LPDIRECT3DDEVICE9 g_pd3dDevice);