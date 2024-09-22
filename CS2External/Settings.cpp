#include "Settings.h";
#include <ImGui/imgui.h>;

extern int width = 1920;
extern int height = 1080;

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
		bool Health = true;
		bool Box = true;
		bool Lines = true;
	}
}