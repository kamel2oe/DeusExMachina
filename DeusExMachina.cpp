#include <string>
#include <random>
#include <set>

#include "framework.h"
#include "DeusExMachina.h"
#include "Memory.h"
#include "omp/EquityCalculator.h"

// Forward declarations of functions included in this code module:
BOOL                InitInstance(HINSTANCE);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
int width = 1000;
int height = 1000;
uintptr_t base_address;
uintptr_t base_size;
uintptr_t table_manager;

bool do_once = true;

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

float GetEquity(std::string my_hand, int players, std::string cards)
{
    printf("hand: %s cards: %s, players: %i\n", my_hand.c_str(), cards.c_str(), players);

    omp::EquityCalculator eq;

    std::vector<omp::CardRange> hand_ranges;

    hand_ranges.push_back(my_hand);

    for (int i = 0; i < players; i++) {
        hand_ranges.push_back("random");
    }

    eq.start(hand_ranges, omp::CardRange::getCardMask(cards), 0, false, 5e-5, nullptr, 0.2, 0);
    eq.wait();
    auto r = eq.getResults();

    return r.equity[0];
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    OpenConsole();

    mem->FindProcess("PokerStars.exe");
    mem->Open();

    MODULEENTRY32 module = mem->FindModule("PokerStars.exe");

    base_address = (uintptr_t)module.modBaseAddr;
    base_size = module.modBaseSize;
    printf("[base_address] %X [size] %X\n", base_address, base_size);

    //mov eax,["PokerStars.exe"+12D8994]
    //AppModule : CommClientAuthCallback
    uint32_t table_manager_offset = mem->FindPattern(module, "\xa1\x00\x00\x00\x00\x85\xc0\x74\x00\x8b\xe5\x5d\xc3\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x8d\x4d\x00\xe8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x8d\x45\x00\x50\xe8\x00\x00\x00\x00\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\x55\x8b\xec\x51", "x????xxx?xxxxx????x????x????xx?x????x????xx?xx????xxxxxxxxxxxxxx") + 1;
    printf("[table_manager_offset] 0x%X\n", table_manager_offset);
    printf("[table_manager_offset] 0x%X\n", table_manager_offset - base_address);

    uint32_t table_manager_func = mem->Read<uint32_t>(table_manager_offset);
    printf("[table_manager_func] 0x%X\n", table_manager_func);
    printf("[table_manager_func - base_address] 0x%X\n", table_manager_func - base_address);

    table_manager = mem->Read<uint32_t>(table_manager_func);
    printf("[table_manager] 0x%X\n", table_manager);
    printf("[table_manager - base_address] 0x%X\n", table_manager - base_address);

    // Perform application initialization:
    if (!InitInstance(hInstance))
    {
        return FALSE;
    }
}

enum Stages : int
{
    PreFlop = 0,
    Flop = 1,
    Turn = 2,
    River = 3
};

class Player
{
    uintptr_t base;
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

    Player(uintptr_t player_address) {
        base = player_address;
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
};

class Card
{
    uintptr_t base;
public:
    int number;
    int type;

    Card(uintptr_t card_address)
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

void DrawTableInfo(uint32_t table, uint32_t table_client_data, char* table_name)
{
    std::vector<Player> players;
    std::vector<Card> cards;

    uint32_t player_offset = 0xC08;
    uint32_t player_size = 0x1F8;

    uint32_t cards_on_display_count = mem->Read<uint32_t>(table_client_data + 0xB50);
    uint32_t pot_size = mem->Read<uint32_t>(table_client_data + 0x234);

    uint32_t dealer_player_id = mem->Read<uint32_t>(table + 0xF18);
    uint32_t current_player_id = mem->Read<uint32_t>(table + 0xF1C);

    ImGui::Text("Table Name: %s", table_name);

    ImGui::Text("Cards: %i", cards_on_display_count);
    ImGui::Text("Pot: %i", pot_size);

    ImGui::Text("Dealer Player ID: %i", dealer_player_id);
    ImGui::Text("Current Player ID: %i", current_player_id);

    uint32_t ante = mem->Read<uint32_t>(table_client_data + 0x298);
    uint32_t small_blind = mem->Read<uint32_t>(table_client_data + 0xEC);
    uint32_t big_blind = mem->Read<uint32_t>(table_client_data + 0xF0);

    ImGui::Text("Ante %i, Small Blind %i, Big Blind %i", ante, small_blind, big_blind);

    for (int i = 0; i < 5; i++) {
        uint32_t card_address = (table_client_data + 0xB5C) + (0x8 * i);

        Card card(card_address);

        ImGui::Image((void*)GetCardTexture(g_pd3dDevice, card.number, card.type), ImVec2(134 / 3, 186 / 3));

        if (i != 4)
            ImGui::SameLine();

        cards.push_back(card);
    }

    ImGui::Columns(7, "mycolumns");
    ImGui::Separator();
    ImGui::Text("ID"); ImGui::NextColumn();
    ImGui::Text("Name"); ImGui::NextColumn();
    ImGui::Text("Amount"); ImGui::NextColumn();
    ImGui::Text("Current Bet"); ImGui::NextColumn();
    ImGui::Text("Total Bet"); ImGui::NextColumn();
    ImGui::Text("Total"); ImGui::NextColumn();
    ImGui::Text("State"); ImGui::NextColumn();
    ImGui::Separator();

    for (int i = 0; i < 9; i++)
    {
        uint32_t player_address = (table_client_data + 0xC08) + (player_size * i);

        Player player(player_address);

        if (!player.IsValid())
            continue;

        players.push_back(player);

        if (dealer_player_id == i) {
            ImGui::Text("%i D", i);
        }
        else if (dealer_player_id + 1 == i)
        {
            ImGui::Text("%i SB", i);
        }
        else if (dealer_player_id + 2 == i)
        {
            ImGui::Text("%i BB", i);
        }
        else {
            ImGui::Text("%i", i);
        }
        ImGui::NextColumn();

        ImGui::Text("%s", player.name);

        ImGui::Image((void*)GetCardTexture(g_pd3dDevice, player.first_card_number, player.first_card_type), ImVec2(134 / 4, 186 / 4));
        ImGui::SameLine();
        ImGui::Image((void*)GetCardTexture(g_pd3dDevice, player.second_card_number, player.second_card_type), ImVec2(134 / 4, 186 / 4));
        ImGui::NextColumn();

        ImGui::Text("%i", player.chip_size);
        ImGui::NextColumn();

        ImGui::Text("%i", player.bet_amount);
        ImGui::NextColumn();

        ImGui::Text("%i", player.bet_total);
        ImGui::NextColumn();

        ImGui::Text("%i", player.chip_size + player.bet_amount);
        ImGui::NextColumn();

        ImGui::Text("%s", player.state == 1 ? "FOLDED/SITTING OUT" : "IN");
        ImGui::NextColumn();
    }

    std::string player_name = "greenarr0528";

    auto it = std::find_if(players.begin(), players.end(), [&player_name](const Player& obj) {return obj.name == player_name; });

    if (it != players.end())
    {
        auto index = std::distance(players.begin(), it);

        Player current_player = players.at(index);

        if (current_player.first_card_number != 0 && current_player.second_card_number != 0)
        {
            std::string my_hand = pp_card(current_player.first_card_number, current_player.first_card_type) + pp_card(current_player.second_card_number, current_player.second_card_type);
            std::string table_hand;

            for (Card& card : cards)
            {
                if (card.number != 0)
                {
                    table_hand += pp_card(card.number, card.type);
                }
            }

            //printf("[my_hand] %s\n", my_hand.c_str());
            //printf("[table_hand] %s\n", table_hand.c_str());

            float equity = GetEquity(my_hand, players.size() - 1, table_hand);
            printf("[equity] %f\n", equity);
        }
    }

    ImGui::Columns(1);
    ImGui::Separator();

    players.clear();
    cards.clear();
}

void Draw()
{
    if (do_once) {
        LoadImages(g_pd3dDevice);
        do_once = false;
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);

    ImGui::Begin("##Backbuffer", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    {
        uint32_t normal_tables = mem->Read<uint32_t>(table_manager + 0x14);

        uint32_t unk_0 = mem->Read<uint32_t>(table_manager + 0x20);
        uint32_t tournament_tables = mem->Read<uint32_t>(unk_0 + 0x4);

        std::vector<std::string> table_names;

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            int current_normal_table = 0;

            // Normal tables
            while (true)
            {
                uint32_t table = mem->Read<uint32_t>(normal_tables + (0x4 * current_normal_table));
                uint32_t table_client_data = mem->Read<uint32_t>(table + 0x164);

                int valid = mem->Read<int>(table + 0x2A0);

                if (valid == 0)
                    break;

                char table_name[15];
                uint32_t table_name_ptr = mem->Read<uint32_t>(table_client_data + 0xC4);
                mem->ReadBuffer(table_name_ptr, &table_name, sizeof(table_name));

                if (std::find(table_names.begin(), table_names.end(), table_name) != table_names.end()) {
                    break;
                }
                else {
                    table_names.push_back(table_name);
                }

                if (ImGui::BeginTabItem(table_name))
                {
                    DrawTableInfo(table, table_client_data, table_name);

                    ImGui::EndTabItem();
                }
                current_normal_table++;
            }

            int current_tournament_table = 0;

            //Tournament tables
            while (true)
            {
                uint32_t table = mem->Read<uint32_t>(tournament_tables + (0x4 * current_tournament_table));
                uint32_t table_client_data = mem->Read<uint32_t>(table + 0x164);

                int valid = mem->Read<int>(table + 0x2A0);

                if (valid == 0)
                    break;

                char table_name[15];
                uint32_t table_name_ptr = mem->Read<uint32_t>(table_client_data + 0xC4);
                mem->ReadBuffer(table_name_ptr, &table_name, sizeof(table_name));

                if (std::find(table_names.begin(), table_names.end(), table_name) != table_names.end()) {
                    break;
                }
                else {
                    table_names.push_back(table_name);
                }

                if (ImGui::BeginTabItem(table_name))
                {
                    DrawTableInfo(table, table_client_data, table_name);

                    ImGui::EndTabItem();
                }
                current_tournament_table++;
            }

            ImGui::EndTabBar();
        }
      
        table_names.empty();
    }
    ImGui::End();

    //bool show_demo_window = 1;
    //if (show_demo_window)
    //    ImGui::ShowDemoWindow(&show_demo_window);

    
}

std::string random_string(const int len) {
    const std::string alpha_numeric("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");

    std::default_random_engine generator{ std::random_device{}() };
    const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };

    std::string str(len, 0);
    for (auto& it : str) {
        it = alpha_numeric[distribution(generator)];
    }

    return str;
}

BOOL InitInstance(HINSTANCE hInstance)
{
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("1"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, random_string(12).c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 100, 100, width, height, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    //SetWindowLong(hwnd, GWL_STYLE, 0);
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    io.Fonts->AddFontFromMemoryCompressedTTF(RobotoBlack_compressed_data, 1.0f, 16.0f);

    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.0f);

    // Styles
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        style->Alpha = 0.97f;
        style->WindowMenuButtonPosition = ImGuiDir_None;
        style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 0.0f;
        style->FramePadding = ImVec2(5, 5);
        style->FrameRounding = 0.0f;
        style->ItemSpacing = ImVec2(12, 8);
        style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 25.0f;
        style->ScrollbarSize = 15.0f;
        style->GrabMinSize = 5.0f;
        style->TabRounding = 0.0f;
        style->TabBorderSize = 0.0f;
        style->FrameBorderSize = 1.0f;
        style->ChildRounding = 0.0f;
        style->PopupRounding = 0.0f;
        style->ScrollbarRounding = 0.0f;
        style->GrabRounding = 0.0f;
    }
    // Colors
    {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.14f, 0.16f, 0.11f, 0.52f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.30f, 0.23f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.34f, 0.26f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.32f, 0.24f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.30f, 0.22f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.23f, 0.27f, 0.21f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
        colors[ImGuiCol_Button] = ImVec4(0.29f, 0.34f, 0.26f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
        colors[ImGuiCol_Header] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.42f, 0.31f, 0.6f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
        colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.11f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.19f, 0.23f, 0.18f, 0.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.54f, 0.57f, 0.51f, 0.78f);
        colors[ImGuiCol_TabActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.78f, 0.28f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.73f, 0.67f, 0.24f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        Draw();

        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return TRUE;
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, message, wParam, lParam);
}
