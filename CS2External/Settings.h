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
		extern bool ThirdPerson;
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
		extern bool AllEntity;
		extern bool Item;
		extern bool Chiken;
		extern bool Bomb;
		extern bool Glow;
	}

	namespace Aimbot
	{
		extern const char* items[15];
		extern int Key;
		extern bool Aimbot;
		extern bool RCS;
		extern bool Slient;
		extern bool Smooth;
		extern float SmoothValue;
		extern bool Team;
		extern bool VisibilityCheck;
		extern bool Trigger;
		extern float TriggerDelay;
		extern float BetweenShotDelay;
		extern int SelectedItem;
		extern bool distance_angle_switch;
		extern bool FOVCheck;
		extern float FOV;
	}
}