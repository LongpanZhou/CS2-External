#pragma once

class ImColor;

namespace Settings
{
	extern bool showMenu;
	namespace Util
	{
		extern bool Bhop;
		extern bool Radar;
		extern bool NoFlash;
	}
	namespace ESP
	{
		extern bool ESP;
		extern bool Team;
		extern bool Health;
		extern bool Name;
		extern bool Weapon;
		extern bool Location;
		extern bool Box;
		extern bool Lines;
		extern bool bone;
		extern bool Distance;
		extern bool VisionProjection;
	}

	namespace Misc
	{
		extern bool Item;
		extern bool Bomb;
		extern bool Glow;
	}

	namespace Aimbot
	{
		extern const char* items[15];
		extern bool Aimbot;
		extern bool slient;
		extern bool Team;
		extern bool VisibilityCheck;
		extern bool Trigger;
		extern int SelectedItem;
		extern bool distance_angle_switch;
		extern bool FOVCheck;
		extern float FOV;
		extern int AimKey;
	}
}