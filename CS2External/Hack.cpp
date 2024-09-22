#include "Hack.h"
#include <geom.h>
#include "Settings.h"
#include "Offsets.h"
#include <Windows.h>
#include "Mem.h"
#include <thread>
#include <iostream>
#include <ImGui/imgui.h>

auto mem = Mem(L"cs2.exe");
auto baseAddress = mem.GetModuleAddress(L"client.dll");
auto serverBaseAddress = mem.GetModuleAddress(L"server.dll");
uintptr_t localPlayerPtr = mem.ReadMemory<uintptr_t>(baseAddress + Offsets::dwLocalPlayer);

void Hack::Util::Bhop()
{
	if (Settings::Util::Bhop && GetAsyncKeyState(VK_SPACE))
	{
		int jump_state = mem.ReadMemory<int>(baseAddress + Offsets::dwJumpState);
		mem.WriteMemory<int>(baseAddress + Offsets::dwForceJump, 65537);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		mem.WriteMemory<int>(baseAddress + Offsets::dwForceJump, 16777472);
	}
}

void Hack::ESP::ESP()
{
	if (Settings::ESP::ESP)
	{
		int numPlayers = mem.ReadMemory<int>(serverBaseAddress + Offsets::NumOfPlayers);

		float LocalViewMatrix[16];
		mem.ReadMemory(baseAddress + Offsets::ViewMatrix, LocalViewMatrix, 16);

		for (int i = 1; i < numPlayers; i++)
		{
			uintptr_t entity = mem.ReadMemory<uintptr_t>(baseAddress + Offsets::dwEntityList + i * 0x10);
			int health = mem.ReadMemory<int>(entity + Offsets::LocalPlayer::Health);
			
			if (health <= 0 || health > 100)
				continue;
			
			Vec3 feet = mem.ReadMemory<Vec3>(entity + Offsets::LocalPlayer::Position);
			Vec3 head = mem.ReadMemory<Vec3>(entity + Offsets::LocalPlayer::HeadPosition);

			bool Draw = false;
			Vec3 wtsFeet, wtsHead;
			Draw |= T_WorldToScreen(feet, wtsFeet, LocalViewMatrix,width,height);
			Draw |= T_WorldToScreen(head, wtsHead, LocalViewMatrix, width, height);
			
			if (!Draw)
				continue;

			if (Settings::ESP::Lines)
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(width / 2, height), ImVec2(wtsFeet.x, wtsFeet.y), ImColor(255, 255, 255, 255));

			Vec4 ESPrect = CalcRect(wtsFeet, wtsHead);
			if (Settings::ESP::Box)
				ImGui::GetForegroundDrawList()->AddRect({ ESPrect.x,ESPrect.y }, { ESPrect.z, ESPrect.w }, ImColor(255, 0, 0));

			if (Settings::ESP::Health)
			{
				ImVec2 start{ ESPrect.x + 5, wtsFeet.y };
				ImVec2 end{ ESPrect.x + 5, wtsFeet.y + (wtsHead.y - wtsFeet.y) * (health / 100.f) };
				ImColor color{ (int)(255 * (1 - health / 100.f)), (int)(255 * health / 100.f), 0 };
				ImGui::GetForegroundDrawList()->AddLine(start, end, color, 3.0f);
			}
		}
	}
}

void Hack::hackloop()
{
	Hack::Util::Bhop();
	Hack::ESP::ESP();
}