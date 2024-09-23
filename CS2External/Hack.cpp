#include "Hack.h"
#include <geom.h>
#include "Settings.h"
#include "Offsets.h"
#include <Windows.h>
#include "Mem.h"
#include <thread>
#include <iostream>
#include <ImGui/imgui.h>
#include "bone.h"

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
			if (!entity)
				continue;
			
			int health = mem.ReadMemory<int>(entity + Offsets::LocalPlayer::Health);

			if (health <= 0 || health > 100)
				continue;
			
			Vec3 feet = mem.ReadMemory<Vec3>(entity + Offsets::LocalPlayer::Position);
			Vec3 head = mem.ReadMemory<Vec3>(entity + Offsets::LocalPlayer::HeadPosition);

			bool Draw = false;
			Vec3 wtsFeet, wtsHead;
			Draw |= T_WorldToScreen(feet, wtsFeet, LocalViewMatrix, width, height);
			Draw |= T_WorldToScreen(head + Vec3{0,0,10}, wtsHead, LocalViewMatrix, width, height);
			
			if (!Draw)
				continue;

			if (Settings::ESP::Lines)
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(width / 2, height), ImVec2(wtsFeet.x, wtsFeet.y), ImColor(255, 255, 255, 255));

			Vec4 ESPrect = CalcRect(wtsFeet, wtsHead);
			if (Settings::ESP::Box)
				ImGui::GetForegroundDrawList()->AddRect({ ESPrect.x,ESPrect.y }, { ESPrect.z, ESPrect.w }, ImColor(255, 255, 255));

			if (Settings::ESP::Health)
			{
				ImVec2 start{ ESPrect.x + 5, wtsFeet.y };
				ImVec2 end{ ESPrect.x + 5, wtsFeet.y + (wtsHead.y - wtsFeet.y) * (health / 100.f) };
				ImVec2 fillend{ ESPrect.x + 5, wtsHead.y };

				ImColor color = (health > 50)
					? ImColor((int)(255 * (1 - (health - 50) / 50.0f)), 255, 0)
					: ImColor(255, (int)(126 * (health / 50.0f)), 0);

				ImGui::GetForegroundDrawList()->AddLine(start, fillend, ImColor(105, 105, 105), 3);
				ImGui::GetForegroundDrawList()->AddLine(start, end, color, 3.0f);
			}

			if (Settings::ESP::bone)
			{
				uintptr_t gamescene = mem.ReadMemory<uintptr_t>(entity + Offsets::LocalPlayer::m_GameSceneNode);
				uintptr_t boneArrayptr = mem.ReadMemory<uintptr_t>(gamescene + Offsets::GameSceneNode::boneMatrix);

				BoneJointData boneArray[28];
				mem.ReadMemory<BoneJointData>(boneArrayptr,boneArray,28);
				
				std::vector<vec3> bonePositions(28, vec3());

				std::vector<int> boneIndices = {
					BONEINDEX::PELVIS,
					BONEINDEX::NECK,
					BONEINDEX::HEAD,
					BONEINDEX::LEFT_SHOULDER,
					BONEINDEX::LEFT_ELBOW,
					BONEINDEX::LEFT_HAND,
					BONEINDEX::RIGHT_SHOULDER,
					BONEINDEX::RIGHT_ELBOW,
					BONEINDEX::RIGHT_HAND,
					BONEINDEX::LEFT_LEG,
					BONEINDEX::LEFT_KNEE,
					BONEINDEX::LEFT_FOOT,
					BONEINDEX::RIGHT_LEG,
					BONEINDEX::RIGHT_KNEE,
					BONEINDEX::RIGHT_FOOT
				};

				for (int index : boneIndices) {
					T_WorldToScreen(boneArray[index].Pos, bonePositions[index], LocalViewMatrix, width, height);
				}

				float scaling_value = bonePositions[BONEINDEX::HEAD].Distance(bonePositions[BONEINDEX::NECK]);
				ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(bonePositions[6].x, bonePositions[6].y), scaling_value*0.75, ImColor(255, 0, 0), 32);

				for (BoneConnection bones : Skeleton) {
					ImGui::GetForegroundDrawList()->AddLine(
						ImVec2(bonePositions[bones.bone_start].x, bonePositions[bones.bone_start].y),
						ImVec2(bonePositions[bones.bone_end].x, bonePositions[bones.bone_end].y),
						ImColor(255, 255, 255, 255)
					);
				}
			}
		}
	}
}

void Hack::hackloop()
{
	Hack::Util::Bhop();
	Hack::ESP::ESP();
}