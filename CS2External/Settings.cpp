#include "Settings.h";
#include <ImGui/imgui.h>;

namespace Settings
{
	bool showMenu = true;
	namespace Util
	{
		bool Bhop = false;
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
}