#include "Menu.h"
#include "Settings.h"
#include <ImGui/imgui.h>
#include <GLFW/glfw3.h>

void Menu::ToggleMenu()
{
	Settings::showMenu = !Settings::showMenu;
	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureMouse = Settings::showMenu;
	io.WantCaptureKeyboard = Settings::showMenu;
	io.MouseDrawCursor = Settings::showMenu;
}

void Menu::Utility()
{
    if (ImGui::BeginTabItem("Utility"))
    {
        ImGui::Checkbox("Bhop", &Settings::Util::Bhop);
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
            Utility();

            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}