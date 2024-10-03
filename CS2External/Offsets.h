#pragma once

namespace Offsets
{
	constexpr unsigned int dwLocalPlayerPawn = 0x18220C8;
	constexpr unsigned int dwEntityList = 0x19BAC58;
	constexpr unsigned int dwJumpState = 0x0017BA518;
	constexpr unsigned int dwForceJump = 0x17BA530;
	constexpr unsigned int dwViewMatrix = 0x1A1CD70;

	namespace LocalPlayer
	{
		constexpr unsigned int m_iHealth = 0x344;
		constexpr unsigned int m_vLastSlopeCheckPos = 0xDB0;
		constexpr unsigned int m_angEyeAngles = 0x1430;
		constexpr unsigned int m_vecLastClipCameraPos = 0x137C;
		constexpr unsigned int m_pGameSceneNode = 0x328;
		constexpr unsigned int m_iTeamNum = 0x3E3;
		constexpr unsigned int m_szLastPlaceName = 0x154C;
	}

	namespace GameSceneNode
	{
		constexpr unsigned int boneMatrix = 0x1F0;
		constexpr unsigned int m_vecOrigin = 0x88;
	}

	namespace BasePlayerPawn
	{
		constexpr unsigned int m_hController = 0x1334;
		constexpr unsigned int m_pClippingWeapon = 0x1398;
		constexpr unsigned int m_vOldOrigin = 0x131C;
		constexpr unsigned int m_AttributeManager = 0x1140;
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
}