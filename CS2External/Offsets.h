#pragma once

namespace Offsets
{
	constexpr unsigned int dwLocalPlayerPawn = 0x1823128;
	constexpr unsigned int dwEntityList = 0x19BBCC8;
	constexpr unsigned int dwForceJump = 0x181C130;
	constexpr unsigned int dwViewMatrix = 0x1A1DDE0;
	constexpr unsigned int dwWeaponC4 = 0x19BEDF0;
	constexpr unsigned int dwPlantedC4 = 0x1A21F28;
	constexpr unsigned int dwViewAngles = 0x1A27C50;

	namespace LocalPlayer
	{
		constexpr unsigned int m_iHealth = 0x344;
		constexpr unsigned int m_vLastSlopeCheckPos = 0xDB0;
		constexpr unsigned int m_angEyeAngles = 0x1430;
		constexpr unsigned int m_vecLastClipCameraPos = 0x137C;
		constexpr unsigned int m_pGameSceneNode = 0x328;
		constexpr unsigned int m_iTeamNum = 0x3E3;
		constexpr unsigned int m_szLastPlaceName = 0x154C;
		constexpr unsigned int m_fFlags = 0x3EC;
		constexpr unsigned int m_iIDEntIndex = 0x1450;
		constexpr unsigned int m_entitySpottedState = 0x23A8;
		constexpr unsigned int m_flFlashMaxAlpha = 0x1400;
	}

	namespace EntitySpottedState_t {
		constexpr unsigned int m_bSpotted = 0x8;
		constexpr unsigned int m_bSpottedByMask = 0xC;
	}

	namespace GameSceneNode
	{
		constexpr unsigned int boneMatrix = 0x1F0;
		constexpr unsigned int m_vecOrigin = 0x88;
		constexpr unsigned int m_vecAbsOrigin = 0xD0;
	}

	namespace BasePlayerPawn
	{
		constexpr unsigned int m_hController = 0x1334;
		constexpr unsigned int m_pClippingWeapon = 0x1398;
		constexpr unsigned int m_vOldOrigin = 0x131C;
		constexpr unsigned int m_AttributeManager = 0x1140;
		constexpr unsigned int m_entitySpottedState = 0x23A8;
	}

	namespace CSPlayerController
	{
		constexpr unsigned int m_hPlayerPawn = 0x80C;
		constexpr unsigned int m_sSanitizedPlayerName = 0x770;
	}

	namespace EconItemView
	{
		constexpr unsigned int m_iItemDefinitionIndex = 0x1BA;
	}

	namespace AttributeContainer
	{
		constexpr unsigned int m_Item = 0x50;
	}

	namespace C_BaseModelEntity
	{
		constexpr unsigned int m_Glow = 0xBF8;
	}

	namespace CGlowProperty
	{
		constexpr unsigned int m_bGlowing = 0x51;
		constexpr unsigned int m_glowColorOverride = 0x40;
		constexpr unsigned int m_iGlowType = 0x30;
	}
}