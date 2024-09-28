#pragma once

namespace Offsets
{
	constexpr unsigned int dwLocalPlayer = 0x17C17F0;
	constexpr unsigned int dwEntityList = 0x17CEB00;
	constexpr unsigned int dwJumpState = 0x0017BA518;
	constexpr unsigned int dwForceJump = 0x17BA530;
	constexpr unsigned int ViewMatrix = 0x19B64F0;
	constexpr unsigned int NumOfPlayers = 0x138FE54;

	namespace LocalPlayer
	{
		constexpr unsigned int Health = 0x324;
		constexpr unsigned int Position = 0xD58;
		constexpr unsigned int ViewAngle = 0x1388;
		constexpr unsigned int HeadPosition = 0x12D4;
		constexpr unsigned int m_GameSceneNode = 0x308;
		constexpr unsigned int m_iTeamNum = 0x3C3;
		constexpr unsigned int m_szLastPlaceName = 0x14A4;
	}

	namespace GameSceneNode
	{
		constexpr unsigned int boneMatrix = 0x1F0;
		constexpr unsigned int m_vecOrigin = 0x88;
	}

	namespace BasePlayerController
	{
		constexpr unsigned int m_iszPlayerName = 0x640;
	}

	namespace BasePlayerPawn
	{
		constexpr unsigned int m_hController = 0x128C;
		constexpr unsigned int m_pClippingWeapon = 0x12F0;
		constexpr unsigned int m_vOldOrigin = 0x1274;
		constexpr unsigned int m_AttributeManager = 0x1090;
	}

	namespace CSPlayerController
	{
		constexpr unsigned int m_hPlayerPawn = 0x7EC;
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