#include <Windows.h>
#include <iostream>
#include <geom.h>
#include "Menu.h"
#include "Settings.h"
#include "Hack.h"
#include <Discord/ImGui_setup.hpp>

RECT rect;
HWND game;
bool insert_pressed = false;

int main()
{
	std::cout << "Insert: Toggle Menu" << std::endl;
	std::cout << "Delete: Exit" << std::endl;

	game = FindWindowA(NULL, "Counter-Strike 2");
	getGameRect(game,rect);
	
	UINT targetProcessId;
	GetWindowThreadProcessId(game, reinterpret_cast<LPDWORD>(&targetProcessId));

	Discord::ConnectedProcessInfo processInfo = { 0 };
	processInfo.ProcessId = targetProcessId;
	Discord::ConnectToProcess(processInfo);

	ImGui_Setup(game);

    while (true) {
		RECT rect;
		GetClientRect(game, &rect);
		UINT width = rect.right - rect.left;
		UINT height = rect.bottom - rect.top;

        HandleToggle(std::bind(&Menu::ToggleMenu));
		if (GetAsyncKeyState(VK_DELETE))
		{
			ImGui_NewFrame();
			ImGui_EndFrame();
			Discord::SendBuffer(processInfo.MappedAddress, processInfo.File, g_pd3dDeviceContext, g_pSwapChain, width, height);
			break;
		}

        ImGui_NewFrame();
        Menu::MainMenu();
        Hack::hackloop();
        ImGui_EndFrame();

        resize(width, height);
        Discord::SendBuffer(processInfo.MappedAddress, processInfo.File, g_pd3dDeviceContext, g_pSwapChain, width, height);
        ImGui_MouseEvent();
    }
	ImGui_Shutdown();
	Discord::DisconnectFromProcess(processInfo);
	return 0;
}