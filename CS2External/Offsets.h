#include "cs2-dumper/client_dll.hpp"
#include "cs2-dumper/offsets.hpp"
#pragma once

// First time dealing with massive offsets please ignore garbage code
namespace Offsets
{
	constexpr unsigned int dwLocalPlayerPawn = cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;
	constexpr unsigned int dwEntityList = cs2_dumper::offsets::client_dll::dwEntityList;
	constexpr unsigned int dwViewMatrix = cs2_dumper::offsets::client_dll::dwViewMatrix;
	constexpr unsigned int dwWeaponC4 = cs2_dumper::offsets::client_dll::dwWeaponC4;
	constexpr unsigned int dwPlantedC4 = cs2_dumper::offsets::client_dll::dwPlantedC4;
	constexpr unsigned int dwViewAngles = cs2_dumper::offsets::client_dll::dwViewAngles;
	constexpr unsigned int dwCSGOInput = cs2_dumper::offsets::client_dll::dwCSGOInput;
	constexpr unsigned int dwForceJump = 0x181E140; //CUSTOM
	constexpr unsigned int dwForceAttack = 0x181DC32; //CUSTOM

	constexpr unsigned int loadViewAngleFuncYaw = 0x89F137; //R.I.P
	constexpr unsigned int loadViewAngleFuncPitch = 0x89636D; //R.I.P

	namespace CSGOInput //CUSTOM
	{
		constexpr unsigned int dwCameraInThirdPerson = 0x250; 
		constexpr unsigned int dwViewAngles = 0x258;
		constexpr unsigned int patchAddress = 0x76A43D;
	}

	namespace LocalPlayer
	{
		constexpr unsigned int m_iHealth = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth;
		constexpr unsigned int m_vLastSlopeCheckPos = cs2_dumper::schemas::client_dll::CBaseAnimGraph::m_vLastSlopeCheckPos;
		constexpr unsigned int m_angEyeAngles = cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_angEyeAngles;
		constexpr unsigned int m_vecLastClipCameraPos = cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_vecLastClipCameraPos;
		constexpr unsigned int m_pGameSceneNode = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode;
		constexpr unsigned int m_iTeamNum = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum;
		constexpr unsigned int m_szLastPlaceName = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_szLastPlaceName;
		constexpr unsigned int m_fFlags = cs2_dumper::schemas::client_dll::C_BaseEntity::m_fFlags;
		constexpr unsigned int m_iIDEntIndex = cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_iIDEntIndex;
		constexpr unsigned int m_flFlashMaxAlpha = cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_flFlashMaxAlpha;
	}

	namespace EntitySpottedState_t {
		constexpr unsigned int m_bSpotted = cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpotted;
		constexpr unsigned int m_bSpottedByMask = cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask;
	}

	namespace GameSceneNode
	{
		constexpr unsigned int boneMatrix = 0x1F0; //CUSTOM
		constexpr unsigned int m_vecOrigin = cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecOrigin;
		constexpr unsigned int m_vecAbsOrigin = cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecAbsOrigin;
	}

	namespace BasePlayerPawn
	{
		constexpr unsigned int m_hController = cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_hController;
		constexpr unsigned int m_pClippingWeapon = cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_pClippingWeapon;
		constexpr unsigned int m_vOldOrigin = cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin;
		constexpr unsigned int m_AttributeManager = cs2_dumper::schemas::client_dll::C_EconEntity::m_AttributeManager;
		constexpr unsigned int m_entitySpottedState = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState;
	}

	namespace CSPlayerController
	{
		constexpr unsigned int m_hPlayerPawn = cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn;
		constexpr unsigned int m_sSanitizedPlayerName = cs2_dumper::schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName;
	}

	namespace EconItemView
	{
		constexpr unsigned int m_iItemDefinitionIndex = cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex;
	}

	namespace AttributeContainer
	{
		constexpr unsigned int m_Item = cs2_dumper::schemas::client_dll::C_AttributeContainer::m_Item;
	}

	namespace C_BaseModelEntity
	{
		constexpr unsigned int m_Glow = cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_Glow;
	}

	namespace CGlowProperty
	{
		constexpr unsigned int m_bGlowing = cs2_dumper::schemas::client_dll::CGlowProperty::m_bGlowing;
		constexpr unsigned int m_glowColorOverride = cs2_dumper::schemas::client_dll::CGlowProperty::m_glowColorOverride;
		constexpr unsigned int m_iGlowType = cs2_dumper::schemas::client_dll::CGlowProperty::m_iGlowType;
	}
}