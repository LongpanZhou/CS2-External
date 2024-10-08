#include "Menu.h"
#include "Settings.h"
#include <ImGui/imgui.h>
#include <GLFW/glfw3.h>
#include "Hack.h"

void Menu::ToggleMenu()
{
	Settings::showMenu = !Settings::showMenu;
	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureMouse = Settings::showMenu;
	io.WantCaptureKeyboard = Settings::showMenu;
	io.MouseDrawCursor = Settings::showMenu;
}

void Menu::Aimbot()
{
    static bool distanceSelected = Settings::Aimbot::distance_angle_switch;
    static bool angleSelected = !Settings::Aimbot::distance_angle_switch;
	if (ImGui::BeginTabItem("Aimbot"))
	{
		ImGui::Checkbox("Aimbot", &Settings::Aimbot::Aimbot);
        if (ImGui::Checkbox("Slient", &Settings::Aimbot::slient))
        {
            if (!Settings::Aimbot::slient)
                Hack::UnpatchSlient();
            else
				Hack::PatchSlient();
        }
		ImGui::Checkbox("Team", &Settings::Aimbot::Team);
		ImGui::Checkbox("Visibility Check", &Settings::Aimbot::VisibilityCheck);
		//ImGui::Checkbox("Trigger", &Settings::Aimbot::Trigger);
        if (ImGui::Checkbox("Closest Angle Enemy", &angleSelected))
        {
            Settings::Aimbot::distance_angle_switch = !angleSelected;
            distanceSelected = !angleSelected;
        }
        if (ImGui::Checkbox("Closest Distance Enemy", &distanceSelected))
        {
            Settings::Aimbot::distance_angle_switch = distanceSelected;
			angleSelected = !distanceSelected;
        }
		ImGui::Checkbox("FOV Check", &Settings::Aimbot::FOVCheck);
		ImGui::SliderFloat("FOV", &Settings::Aimbot::FOV, 0.0f, 360.0f);
        ImGui::ListBox("Aim Position", &Settings::Aimbot::SelectedItem, Settings::Aimbot::items, IM_ARRAYSIZE(Settings::Aimbot::items));
      	ImGui::EndTabItem();
	}
}

void Menu::MISC()
{
	if (ImGui::BeginTabItem("MISC"))
	{
        ImGui::Checkbox("Item", &Settings::Misc::Item);
        ImGui::Checkbox("Glow", &Settings::Misc::Glow);
		ImGui::Checkbox("Bomb", &Settings::Misc::Bomb);
		ImGui::EndTabItem();
	}

}

void Menu::Utility()
{
    if (ImGui::BeginTabItem("Utility"))
    {
        ImGui::Checkbox("Bhop", &Settings::Util::Bhop);
		ImGui::Checkbox("Radar", &Settings::Util::Radar);
		ImGui::Checkbox("NoFlash", &Settings::Util::NoFlash);
        ImGui::EndTabItem();
    }
}

void Menu::ESP()
{
    if (ImGui::BeginTabItem("ESP"))
    {
        ImGui::Checkbox("ESP", &Settings::ESP::ESP);
		ImGui::Checkbox("Team", &Settings::ESP::Team);
		ImGui::Checkbox("Name", &Settings::ESP::Name);
		ImGui::Checkbox("Weapon", &Settings::ESP::Weapon);
        ImGui::Checkbox("Health", &Settings::ESP::Health);
        ImGui::Checkbox("Box", &Settings::ESP::Box);
        ImGui::Checkbox("Lines", &Settings::ESP::Lines);
		ImGui::Checkbox("Bone", &Settings::ESP::bone);
		ImGui::Checkbox("Location", &Settings::ESP::Location);
		ImGui::Checkbox("Vision Projection", &Settings::ESP::VisionProjection);
        ImGui::EndTabItem();
    }
}

void Menu::MainMenu()
{
    if (Settings::showMenu)
    {
        ImGui::Begin("Menu", &Settings::showMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowSize(ImVec2(500, 500));

        if (ImGui::BeginTabBar("Tab bar"))
        {
            ESP();
			MISC();
            Utility();
			Aimbot();

            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}