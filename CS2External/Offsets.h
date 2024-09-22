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
		constexpr unsigned int ViewAngle = 0x119c;
		constexpr unsigned int HeadPosition = 0x12D4;
	}
}