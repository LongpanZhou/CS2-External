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
#include "weapon.hpp"

//I remind you this is not the best optimized code, all the entity loop could have been put in the same loop and other etc...
auto mem = Mem(L"cs2.exe");
auto baseAddress = mem.GetModuleAddress(L"client.dll");
auto serverBaseAddress = mem.GetModuleAddress(L"server.dll");

vec3 aimAngle, PlayerPos, viewAngles;
uintptr_t loadViewAngleFuncYaw = baseAddress + Offsets::loadViewAngleFuncYaw;
uintptr_t loadViewAngleFuncPitch = baseAddress + Offsets::loadViewAngleFuncPitch;
byte originalBytesYaw[10], patchedBytesYaw[10];
byte originalBytesPitch[11], patchedBytesPitch[11];

uintptr_t ThirdPerson = baseAddress + Offsets::CSGOInput::patchAddress;
byte originalThirdPerson[8], patchedThirdPerson[8] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

uintptr_t EntityList = mem.ReadMemory<uintptr_t>(baseAddress + Offsets::dwEntityList);
float LocalViewMatrix[16];
uintptr_t localPlayerPtr;
int localplayer_team;

void Hack::Util()
{
	if (Settings::Util::NoFlash)
	{
		float flashAlpha = mem.ReadMemory<float>(localPlayerPtr + Offsets::LocalPlayer::m_flFlashMaxAlpha);
		if (flashAlpha > 0.0f)
			mem.WriteMemory<float>(localPlayerPtr + Offsets::LocalPlayer::m_flFlashMaxAlpha, 0.0f);
	}

	if (Settings::Util::Bhop)
	{
		int state = mem.ReadMemory<int>(localPlayerPtr + Offsets::LocalPlayer::m_fFlags);

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if (state & 1 << 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				mem.WriteMemory<int>(baseAddress + Offsets::dwForceJump, 65537);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				mem.WriteMemory<int>(baseAddress + Offsets::dwForceJump, 16777472);
			}
		}
	}

	if (Settings::Util::ThirdPerson)
	{
		mem.WriteMemory<Vec3>(baseAddress + Offsets::dwCSGOInput + Offsets::CSGOInput::dwViewAngles, { viewAngles.x, viewAngles.y, 150});
	}
}

void Hack::ESP()
{
	//issues with flashing
	if (Settings::Aimbot::FOVCheck && Settings::Aimbot::Aimbot)
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2{ width / 2.0f, height / 2.0f }, Settings::Aimbot::FOV, ImColor(255, 255, 255, 128), 0.5);

	if (Settings::ESP::ESP)
	{
		std::map<uintptr_t, bool> entityMap;

		for (int i = 1; i < 64; i++)
		{
			uintptr_t listEntityController = mem.ReadMemory<uintptr_t>(EntityList + ((8 * (i & 0x7FFF) >> 9) + 16));
			uintptr_t entityController = mem.ReadMemory<uintptr_t>(listEntityController + (120) * (i & 0x1FF));

			uintptr_t entityControllerPawn = mem.ReadMemory<uintptr_t>(entityController + Offsets::CSPlayerController::m_hPlayerPawn);
			uintptr_t listEntity = mem.ReadMemory<uintptr_t>(EntityList + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16));
			uintptr_t entity = mem.ReadMemory<uintptr_t>(listEntity + (120) * (entityControllerPawn & 0x1FF));

			if (!entity || entity == localPlayerPtr || entityMap.find(entity) != entityMap.end())
				continue;

			entityMap[entity] = true;

			int entity_team = mem.ReadMemory<int>(entity + Offsets::LocalPlayer::m_iTeamNum);

			if (!Settings::ESP::Team && entity_team == localplayer_team)
				continue;

			int health = mem.ReadMemory<int>(entity + Offsets::LocalPlayer::m_iHealth);

			if (health <= 0 || health > 100)
				continue;
			
			Vec3 feet = mem.ReadMemory<Vec3>(entity + Offsets::LocalPlayer::m_vLastSlopeCheckPos);
			Vec3 head = mem.ReadMemory<Vec3>(entity + Offsets::LocalPlayer::m_vecLastClipCameraPos);

			bool Draw = false;
			Vec3 wtsFeet, wtsHead;
			Draw |= T_WorldToScreen(feet, wtsFeet, LocalViewMatrix, width, height);
			Draw |= T_WorldToScreen(head + Vec3{0,0,10}, wtsHead, LocalViewMatrix, width, height);
			
			if (!Draw)
				continue;

			if (Settings::ESP::Lines)
			{
				if (Settings::Util::ThirdPerson)
				{
					Vec3 PlayerFeetPos = mem.ReadMemory<Vec3>(localPlayerPtr + Offsets::LocalPlayer::m_vLastSlopeCheckPos);
					Vec3 PlayerScreenPos;
					T_WorldToScreen(PlayerFeetPos, PlayerScreenPos, LocalViewMatrix, width, height);
					ImGui::GetForegroundDrawList()->AddLine({PlayerScreenPos.x, PlayerScreenPos.y}, { wtsHead.x, wtsHead.y }, ImColor(255, 255, 255));
				}
				else
				{
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(width / 2, height), { wtsHead.x, wtsHead.y }, ImColor(255, 255, 255));
				}
			}

			Vec4 ESPrect = CalcRect(wtsFeet, wtsHead);
			if (Settings::ESP::Box)
				ImGui::GetForegroundDrawList()->AddRectFilled({ ESPrect.x,ESPrect.y }, { ESPrect.z, ESPrect.w }, ImColor(0, 0, 0, 32));

			if (Settings::ESP::Location)
			{
				char Location[14];
				mem.ReadMemory(entity + Offsets::LocalPlayer::m_szLastPlaceName, Location, 14);
				ImGui::GetForegroundDrawList()->AddText({ ESPrect.z, ESPrect.y }, ImColor(255, 255, 255), Location);
			}

			if (Settings::ESP::Name)
			{
				char nameBuffer[128];
				uintptr_t namePtr = mem.ReadMemory<uintptr_t>(entityController + Offsets::CSPlayerController::m_sSanitizedPlayerName);
				mem.ReadMemory(namePtr, nameBuffer, 128);
				ImGui::GetForegroundDrawList()->AddText({ ESPrect.z, ESPrect.w - 15 }, ImColor(255, 255, 255), nameBuffer);
			}

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

			if (Settings::ESP::bone || Settings::ESP::VisionProjection)
			{
				uintptr_t gamescene = mem.ReadMemory<uintptr_t>(entity + Offsets::LocalPlayer::m_pGameSceneNode);
				uintptr_t boneArrayptr = mem.ReadMemory<uintptr_t>(gamescene + Offsets::GameSceneNode::boneMatrix);
				BoneJointData boneArray[28];
				mem.ReadMemory<BoneJointData>(boneArrayptr, boneArray, 28);

				if (Settings::ESP::bone)
				{
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
					ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(bonePositions[6].x, bonePositions[6].y), scaling_value * 0.75, ImColor(255, 0, 0), 32);

					for (BoneConnection bones : Skeleton) {
						ImGui::GetForegroundDrawList()->AddLine(
							ImVec2(bonePositions[bones.bone_start].x, bonePositions[bones.bone_start].y),
							ImVec2(bonePositions[bones.bone_end].x, bonePositions[bones.bone_end].y),
							ImColor(255, 255, 255, 255)
						);
					}
				}

				if (Settings::ESP::VisionProjection)
				{
					Vec2 Entity_ViewAngle = mem.ReadMemory<Vec2>(entity + Offsets::LocalPlayer::m_angEyeAngles);
					Vec3 Vision_Start, Vision_End, Tmp;
					T_WorldToScreen(boneArray[BONEINDEX::HEAD].Pos, Vision_Start, LocalViewMatrix, width, height);
					
					float LineLength = cos(Entity_ViewAngle.x * PI / 180) * 50.f;

					Tmp.x = boneArray[BONEINDEX::HEAD].Pos.x + cos(Entity_ViewAngle.y * PI / 180) * LineLength;
					Tmp.y = boneArray[BONEINDEX::HEAD].Pos.y + sin(Entity_ViewAngle.y * PI / 180) * LineLength;
					Tmp.z = boneArray[BONEINDEX::HEAD].Pos.z - sin(Entity_ViewAngle.x * PI / 180) * 1.0f;

					T_WorldToScreen(Tmp, Vision_End, LocalViewMatrix, width, height);
					
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(Vision_Start.x, Vision_Start.y), ImVec2(Vision_End.x, Vision_End.y), ImColor(255, 0, 0, 255));
				}

				if (Settings::ESP::Weapon)
				{
					uintptr_t pClippingWeapon = mem.ReadMemory<uintptr_t>(entity + Offsets::BasePlayerPawn::m_pClippingWeapon);
					short weaponIndex = mem.ReadMemory<short>(pClippingWeapon + Offsets::AttributeContainer::m_Item + Offsets::EconItemView::m_iItemDefinitionIndex + Offsets::BasePlayerPawn::m_AttributeManager);

					if (ESPrect.x - ESPrect.z > 50)
					{
						ImGuiIO& io = ImGui::GetIO();
						ImFont* weapon_font = io.Fonts->Fonts[1];
						ImGui::PushFont(weapon_font);

						auto it = gunIcons.find(weaponIndex);

						if (it != gunIcons.end()) {
							std::string iconText = it->second;
							ImGui::GetForegroundDrawList()->AddText(
								ImVec2(ESPrect.z, ESPrect.w),
								ImColor(255, 255, 255),
								iconText.c_str()
							);
						}
						ImGui::PopFont();
					}
					else
					{
						ImGui::GetForegroundDrawList()->AddText({ ESPrect.z, ESPrect.w }, ImColor(255, 255, 255), Weapon[weaponIndex].c_str());
					}
				}

				if (Settings::Util::Radar)
				{
					mem.WriteMemory<int>(entity + Offsets::BasePlayerPawn::m_entitySpottedState + Offsets::EntitySpottedState_t::m_bSpotted,Settings::Util::Radar);
				}
			}
		}
	}
}

void Hack::Misc()
{
	if (Settings::Misc::Bomb)
	{
		uintptr_t entity = mem.ReadMemory<uintptr_t>(mem.ReadMemory<uintptr_t>(baseAddress + Offsets::dwWeaponC4));
		uintptr_t gamescene = mem.ReadMemory<uintptr_t>(entity + Offsets::LocalPlayer::m_pGameSceneNode);
		Vec3 EntityPos = mem.ReadMemory<Vec3>(gamescene + Offsets::GameSceneNode::m_vecAbsOrigin);
		Vec3 ScreenPos;
		if (EntityPos.x != 0 || EntityPos.y != 0 || EntityPos.z != 0)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImFont* weapon_font = io.Fonts->Fonts[1];
			ImGui::PushFont(weapon_font);

			if (T_WorldToScreen(EntityPos, ScreenPos, LocalViewMatrix, width, height))
			{
				ImGui::GetForegroundDrawList()->AddText({ ScreenPos.x, ScreenPos.y }, ImColor(255, 255, 255), "o");
			}
			ImGui::PopFont();
		}
	}

	if (Settings::Misc::Glow )
	{
		for (int i = 1; i < 64; i++)
		{
			uintptr_t listEntityController = mem.ReadMemory<uintptr_t>(EntityList + ((8 * (i & 0x7FFF) >> 9) + 16));
			uintptr_t entityController = mem.ReadMemory<uintptr_t>(listEntityController + (120) * (i & 0x1FF));

			uintptr_t entityControllerPawn = mem.ReadMemory<uintptr_t>(entityController + Offsets::CSPlayerController::m_hPlayerPawn);
			uintptr_t listEntity = mem.ReadMemory<uintptr_t>(EntityList + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16));
			uintptr_t entity = mem.ReadMemory<uintptr_t>(listEntity + (120) * (entityControllerPawn & 0x1FF));
		
			if (entity == localPlayerPtr)
				continue;

			int entity_team = mem.ReadMemory<int>(entity + Offsets::LocalPlayer::m_iTeamNum);
			if (!Settings::ESP::Team && entity_team == localplayer_team)
				continue;

			if (entity_team != localplayer_team)
			{
				mem.WriteMemory<int>(entity + Offsets::C_BaseModelEntity::m_Glow + Offsets::CGlowProperty::m_bGlowing, 1);
				mem.WriteMemory<int>(entity + Offsets::C_BaseModelEntity::m_Glow + Offsets::CGlowProperty::m_iGlowType, 3);
				mem.WriteMemory<DWORD>(entity + Offsets::C_BaseModelEntity::m_Glow + Offsets::CGlowProperty::m_glowColorOverride, 0xFF0000FF);
			}
			
			if (entity_team == localplayer_team)
			{
				mem.WriteMemory<int>(entity + Offsets::C_BaseModelEntity::m_Glow + Offsets::CGlowProperty::m_bGlowing, 1);
				mem.WriteMemory<int>(entity + Offsets::C_BaseModelEntity::m_Glow + Offsets::CGlowProperty::m_iGlowType, 3);
				mem.WriteMemory<DWORD>(entity + Offsets::C_BaseModelEntity::m_Glow + Offsets::CGlowProperty::m_glowColorOverride, 0xFFFF0000);
			}
		}
	}

	if (Settings::Misc::Item || Settings::Misc::AllEntity)
	{
		for (int i = 65; i < 1024; i++)
		{
			uintptr_t listEntityController = mem.ReadMemory<uintptr_t>(EntityList + (i < 512 ? 0x10 : 0x18));
			uintptr_t entityController = mem.ReadMemory<uintptr_t>(listEntityController + (120) * (i & 0x1FF));
			if (!entityController)
				continue;

			uintptr_t gamescene = mem.ReadMemory<uintptr_t>(entityController + Offsets::LocalPlayer::m_pGameSceneNode);
			if (!gamescene)
				continue;

			uintptr_t entityIdentity = mem.ReadMemory<uintptr_t>(entityController + 0x10);
			uintptr_t entityNamePtr = mem.ReadMemory<uintptr_t>(entityIdentity + 0x20);

			char entityNameBuffer[64]{};
			mem.ReadMemory(entityNamePtr, entityNameBuffer, 64);
			std::string entityName(entityNameBuffer);

			Vec3 EntityPos = mem.ReadMemory<Vec3>(gamescene + Offsets::GameSceneNode::m_vecOrigin);
			Vec3 ScreenPos;

			if ((EntityPos.x != 0 || EntityPos.y != 0 || EntityPos.z != 0) && (entityName.contains("weapon") || entityName.contains("projectile")) || entityName.contains("baseanimgraph") || (Settings::Misc::Chiken && entityName.contains("chicken")))
			{
				ImGuiIO& io = ImGui::GetIO();
				ImFont* weapon_font = io.Fonts->Fonts[1];
				ImGui::PushFont(weapon_font);

				int EntityIdx = GetIndex(EntityTypeInt, entityName);
				int WeaponIdx = GetIndex(WeaponsTypeInt, entityName);
				int ProjectileIdx = GetIndex(ProjectilesTypeInt, entityName);

				int idx = EntityIdx | WeaponIdx | ProjectileIdx;

				if (T_WorldToScreen(EntityPos, ScreenPos, LocalViewMatrix, width, height))
				{
					ImGui::GetForegroundDrawList()->AddText({ ScreenPos.x, ScreenPos.y }, ImColor(255, 255, 255), gunIcons[idx].c_str());
				}
				ImGui::PopFont();
			}else if (Settings::Misc::AllEntity && (EntityPos.x != 0 || EntityPos.y != 0 || EntityPos.z != 0))
			{
				if (T_WorldToScreen(EntityPos, ScreenPos, LocalViewMatrix, width, height))
				{
					ImGui::GetForegroundDrawList()->AddText({ ScreenPos.x, ScreenPos.y }, ImColor(255, 255, 255), entityNameBuffer);
				}
			}
		}
	}
}

extern void Hack::PatchThird()
{
	DWORD oldProtect;
	VirtualProtect((LPVOID)ThirdPerson, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
	ReadProcessMemory(mem.GetProcessHandle(), (LPVOID)ThirdPerson, originalThirdPerson, sizeof(originalThirdPerson), nullptr);
	WriteProcessMemory(mem.GetProcessHandle(), (LPVOID)ThirdPerson, patchedThirdPerson, sizeof(patchedThirdPerson), nullptr);
	mem.WriteMemory<int>(baseAddress + Offsets::dwCSGOInput + Offsets::CSGOInput::dwCameraInThirdPerson, 256);
	VirtualProtect((LPVOID)ThirdPerson, 8, oldProtect, &oldProtect);
}

extern void Hack::UnpatchThird()
{
	DWORD oldProtect;
	VirtualProtect((LPVOID)ThirdPerson, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
	mem.WriteMemory<int>(baseAddress + Offsets::dwCSGOInput + Offsets::CSGOInput::dwCameraInThirdPerson, 0);
	WriteProcessMemory(mem.GetProcessHandle(), (LPVOID)ThirdPerson, originalThirdPerson, sizeof(originalThirdPerson), nullptr);
	VirtualProtect((LPVOID)ThirdPerson, 8, oldProtect, &oldProtect);
}

extern void Hack::PatchSlient()
{
	DWORD oldProtectYaw, oldProtectPitch;
	VirtualProtect((LPVOID)loadViewAngleFuncYaw, 10, PAGE_EXECUTE_READWRITE, &oldProtectYaw);
	VirtualProtect((LPVOID)loadViewAngleFuncPitch, 11, PAGE_EXECUTE_READWRITE, &oldProtectPitch);

	// mov ebx, x
	// movd xmm0, ebx
	patchedBytesYaw[0] = 0xBA;
	memcpy(&patchedBytesYaw[1], &aimAngle.x, sizeof(float));
	patchedBytesYaw[5] = 0x66;
	patchedBytesYaw[6] = 0x0F;
	patchedBytesYaw[7] = 0x6E;
	patchedBytesYaw[8] = 0xC2;
	patchedBytesYaw[9] = 0x90;

	// mov ebx, y
	// movd xmm1, ebx
	patchedBytesPitch[0] = 0xBA;
	memcpy(&patchedBytesPitch[1], &aimAngle.y, sizeof(float));
	patchedBytesPitch[5] = 0x66;
	patchedBytesPitch[6] = 0x0F;
	patchedBytesPitch[7] = 0x6E;
	patchedBytesPitch[8] = 0xCA;
	patchedBytesPitch[9] = 0x90;
	patchedBytesPitch[10] = 0x90;

	if (originalBytesYaw[0] == 0)
		ReadProcessMemory(mem.GetProcessHandle(), (LPVOID)loadViewAngleFuncYaw, originalBytesYaw, sizeof(originalBytesYaw), nullptr);
	if (originalBytesPitch[0] == 0)
		ReadProcessMemory(mem.GetProcessHandle(), (LPVOID)loadViewAngleFuncPitch, originalBytesPitch, sizeof(originalBytesPitch), nullptr);

	WriteProcessMemory(mem.GetProcessHandle(), (LPVOID)loadViewAngleFuncYaw, patchedBytesYaw, sizeof(patchedBytesYaw), nullptr);
	WriteProcessMemory(mem.GetProcessHandle(), (LPVOID)loadViewAngleFuncPitch, patchedBytesPitch, sizeof(patchedBytesPitch), nullptr);

	VirtualProtect((LPVOID)loadViewAngleFuncYaw, 10, oldProtectYaw, &oldProtectYaw);
	VirtualProtect((LPVOID)loadViewAngleFuncPitch, 11, oldProtectPitch, &oldProtectPitch);
};

extern void Hack::UnpatchSlient()
{
	DWORD oldProtectYaw, oldProtectPitch;
	VirtualProtect((LPVOID)loadViewAngleFuncYaw, 10, PAGE_EXECUTE_READWRITE, &oldProtectYaw);
	VirtualProtect((LPVOID)loadViewAngleFuncPitch, 11, PAGE_EXECUTE_READWRITE, &oldProtectPitch);

	WriteProcessMemory(mem.GetProcessHandle(), (LPVOID)loadViewAngleFuncYaw, originalBytesYaw, sizeof(originalBytesYaw), nullptr);
	WriteProcessMemory(mem.GetProcessHandle(), (LPVOID)loadViewAngleFuncPitch, originalBytesPitch, sizeof(originalBytesPitch), nullptr);

	VirtualProtect((LPVOID)loadViewAngleFuncYaw, 10, oldProtectYaw, &oldProtectYaw);
	VirtualProtect((LPVOID)loadViewAngleFuncPitch, 11, oldProtectPitch, &oldProtectPitch);
};

void Hack::Aimbot()
{
	static std::map<int, int> boneindex = {
										{ 14, 27 },
										{ 13, 26 },
										{ 12, 25 },
										{ 11, 24 },
										{ 10, 23 },
										{ 9, 22 },
										{ 8, 16 },
										{ 7, 14 },
										{ 6, 13 },
										{ 5, 11 },
										{ 4, 9 },
										{ 3, 8 },
										{ 2, 6 },
										{ 1, 5 },
										{ 0, 0 }
	};

	if (Settings::Aimbot::Aimbot)
	{
		uintptr_t ClosestDistanceEnemy, ClosestAngleEnemy;
		float Closestdistance = 999999, Cloestangle = 999999;


		for (int i = 1; i < 64; i++)
		{
			uintptr_t listEntityController = mem.ReadMemory<uintptr_t>(EntityList + ((8 * (i & 0x7FFF) >> 9) + 16));
			uintptr_t entityController = mem.ReadMemory<uintptr_t>(listEntityController + (120) * (i & 0x1FF));

			uintptr_t entityControllerPawn = mem.ReadMemory<uintptr_t>(entityController + Offsets::CSPlayerController::m_hPlayerPawn);
			uintptr_t listEntity = mem.ReadMemory<uintptr_t>(EntityList + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16));
			uintptr_t entity = mem.ReadMemory<uintptr_t>(listEntity + (120) * (entityControllerPawn & 0x1FF));

			if (!entity || entity == localPlayerPtr)
				continue;

			int entity_team = mem.ReadMemory<int>(entity + Offsets::LocalPlayer::m_iTeamNum);

			if (!Settings::Aimbot::Team && entity_team == localplayer_team)
				continue;

			int health = mem.ReadMemory<int>(entity + Offsets::LocalPlayer::m_iHealth);

			if (health <= 0 || health > 100)
				continue;

			Vec3 enemyPos = mem.ReadMemory<Vec3>(entity + Offsets::LocalPlayer::m_vecLastClipCameraPos);

			if (enemyPos.x == 0 || enemyPos.y == 0 || enemyPos.z == 0)
				continue;

			if (Settings::Aimbot::VisibilityCheck)
			{
				int SpottedByMask = mem.ReadMemory<int>(entity + Offsets::BasePlayerPawn::m_entitySpottedState + Offsets::EntitySpottedState_t::m_bSpottedByMask);
				if (!(SpottedByMask & (1 << localPlayerPtr)))
					continue;
			}
			
			Vec3 wtsFeet;
			T_WorldToScreen(enemyPos, wtsFeet, LocalViewMatrix, width, height);
			if (Settings::Aimbot::FOVCheck && abs(Vec3{ width / 2.0f,height / 2.0f, 0 }.Distance(wtsFeet) > Settings::Aimbot::FOV))
				continue;

			float distance = enemyPos.Distance(PlayerPos);
			if (distance < Closestdistance)
			{
				ClosestDistanceEnemy = entity;
				Closestdistance = distance;
			}
			 
			vec3 targetAngle = CalcAngle(PlayerPos, enemyPos);
			float yDiff = targetAngle.y - viewAngles.y;
			if (yDiff > 180)
				yDiff -= 360;
			else if (yDiff < -180)
				yDiff += 360;

			float xDiff = min(abs(viewAngles.x - targetAngle.x), abs(targetAngle.x - viewAngles.x));

			float angleMagnitude = sqrt(pow(xDiff, 2) + pow(yDiff, 2));
			if (angleMagnitude < Cloestangle)
			{
				Cloestangle = angleMagnitude;
				ClosestAngleEnemy = entity;
			}
		}

		Vec3 wtsHead;
		if (!ClosestDistanceEnemy || !ClosestAngleEnemy)
			return;
		
		uintptr_t gamescene = Settings::Aimbot::distance_angle_switch ?
							mem.ReadMemory<uintptr_t>(ClosestDistanceEnemy + Offsets::LocalPlayer::m_pGameSceneNode) :
							mem.ReadMemory<uintptr_t>(ClosestAngleEnemy + Offsets::LocalPlayer::m_pGameSceneNode);

		uintptr_t boneArrayptr = mem.ReadMemory<uintptr_t>(gamescene + Offsets::GameSceneNode::boneMatrix);
		BoneJointData boneArray[28];
		mem.ReadMemory<BoneJointData>(boneArrayptr, boneArray, 28);
		vec3 enemyBonePos = boneArray[boneindex[Settings::Aimbot::SelectedItem]].Pos;

		aimAngle = CalcAngle(PlayerPos, enemyBonePos);
		if (Settings::Aimbot::Slient)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			PatchSlient();
		}
		else if (Settings::Aimbot::Smooth && GetAsyncKeyState(VK_LSHIFT))
		{
			vec3 delta = aimAngle - viewAngles;

			delta.y = aimAngle.y - viewAngles.y;
			if (delta.y > 180)
				delta.y -= 360;
			else if (delta.y < -180)
				delta.y += 360;

			float xDiff = min(abs(aimAngle.x - viewAngles.x), abs(viewAngles.x - aimAngle.x));

			vec3 smoothedAngle = viewAngles + delta*Settings::Aimbot::SmoothValue;
			mem.WriteMemory<vec3>(baseAddress + Offsets::dwViewAngles, smoothedAngle);
		}
		else
		{
			if (GetAsyncKeyState(VK_LSHIFT))
				mem.WriteMemory<Vec3>(baseAddress + Offsets::dwViewAngles, aimAngle);
		}

		if (Settings::Aimbot::Trigger)
		{
			int crosshair_ent = mem.ReadMemory<int>(localPlayerPtr + Offsets::LocalPlayer::m_iIDEntIndex);
			std::this_thread::sleep_for(std::chrono::milliseconds((int)Settings::Aimbot::TriggerDelay * 1000));
			if (Settings::Aimbot::Slient || (crosshair_ent && crosshair_ent != -1))
			{
				mem.WriteMemory<int>(baseAddress + Offsets::dwForceAttack, 1);
				std::this_thread::sleep_for(std::chrono::milliseconds((int)Settings::Aimbot::BetweenShotDelay * 1000));
				mem.WriteMemory<int>(baseAddress + Offsets::dwForceAttack, 0);
			}
		}
	}
}

//Don't really understand how threading works, but it works :/
void Hack::hackloop()
{
	localPlayerPtr = mem.ReadMemory<uintptr_t>(baseAddress + Offsets::dwLocalPlayerPawn);
	localplayer_team = mem.ReadMemory<int>(localPlayerPtr + Offsets::LocalPlayer::m_iTeamNum);
	PlayerPos = mem.ReadMemory<vec3>(localPlayerPtr + Offsets::LocalPlayer::m_vecLastClipCameraPos);
	viewAngles = mem.ReadMemory<vec3>(localPlayerPtr + Offsets::LocalPlayer::m_angEyeAngles);
	mem.ReadMemory(baseAddress + Offsets::dwViewMatrix, LocalViewMatrix, 16);

	std::thread utilThread(Hack::Util);
	utilThread.detach();

	std::thread aimbotThread(Hack::Aimbot);
	aimbotThread.detach();

	Hack::Misc();
	Hack::ESP();
}