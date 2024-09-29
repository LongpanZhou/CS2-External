#include <Windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <geom.h>
#include "Menu.h"
#include "Settings.h"
#include "Hack.h"
#include <filesystem>

RECT rect;
HWND game;
GLFWwindow* window;
bool insert_pressed = false;

#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"opengl32.lib")

void GLFWinit()
{
	glfwInit();
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
}

void GLFW_ImGuiInit()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("weaponIcons.ttf", 16.0f);

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

void renderLoop()
{
	while (!glfwWindowShouldClose(glfwGetCurrentContext()))
	{
		if (GetAsyncKeyState(VK_DELETE))
			break;

		if (GetAsyncKeyState(VK_INSERT) & 0x8000)
		{
			if (!insert_pressed)
			{
				Menu::ToggleMenu();
				insert_pressed = true;
			}
		}
		else
		{
			insert_pressed = false;
		}

		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, !Settings::showMenu);

		getGameRect(game,rect);
		glfwSetWindowPos(window, rect.left, rect.top);
		glfwSetWindowSize(window, width, height);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Menu::MainMenu();
		Hack::hackloop();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
}

void GLFW_ImGuiShutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void GLFW_Shutdown()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main()
{
	std::cout << "Insert: Toggle Menu" << std::endl;
	std::cout << "Delete: Exit" << std::endl;

	game = FindWindowA(NULL, "Counter-Strike 2");
	getGameRect(game,rect);

	GLFWinit();
	window = glfwCreateWindow(width, height, "CS2External", NULL, NULL);
	glfwMakeContextCurrent(window);
	GLFW_ImGuiInit();	

	renderLoop();

	GLFW_ImGuiShutdown();
	GLFW_Shutdown();
}