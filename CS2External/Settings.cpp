#include "Settings.h";
#include <ImGui/imgui.h>;

namespace Settings
{
	bool showMenu = true;
	namespace Util
	{
		bool Bhop = false;
		bool Radar = false;
		bool NoFlash = false;
	}
	namespace ESP
	{
		bool ESP = true;
		bool Team = true;
		bool Health = true;
		bool Name = true;
		bool Weapon = true;
		bool Location = true;
		bool Box = true;
		bool Lines = true;
		bool bone = true;
		bool distance = true;
		bool VisionProjection = true;
	}
	
	namespace Misc
	{
		bool Item = true;
		bool Glow = true;
		bool Bomb = true;
	}

	namespace Aimbot
	{
		const char* items[15] = {
			"Pelvis",
			"Neck",
			"Head",
			"Left Shoulder",
			"Left Elbow",
			"Left Hand",
			"Right Shoulder",
			"Right Elbow",
			"Right Hand",
			"Left Leg",
			"Left Knee",
			"Left Foot",
			"Right Leg",
			"Right Knee",
			"Right Foot"
		};
		bool Aimbot = false;
		bool slient = false;
		bool Team = false;
		bool VisibilityCheck = false;
		bool Trigger = false;
		int SelectedItem = 2;
		bool distance_angle_switch = false;
		bool FOVCheck = true;
		float FOV = 180.0f;
		int AimKey = 0xA0;
	}
}