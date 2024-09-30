#include "Discord.h"
//extern bool show = true;
//bool insert_pressed = false;

static D3D11_MAPPED_SUBRESOURCE		mappedResource			= {};
static ID3D11Device*				g_pd3dDevice			= nullptr;
static ID3D11DeviceContext*			g_pd3dDeviceContext		= nullptr;
static IDXGISwapChain*				g_pSwapChain			= nullptr;
static bool							g_SwapChainOccluded		= false;
static ID3D11RenderTargetView*		g_mainRenderTargetView	= nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

bool CreateDeviceD3D(HWND hWnd, UINT width, UINT height)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

	HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);

	if (res == DXGI_ERROR_UNSUPPORTED)
		res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);

	if (res != S_OK)
	{
		return false;
	}

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer = nullptr;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

void ImGui_Setup(HWND hWnd)
{
	GetClientRect(hWnd, &rect);
	UINT width = rect.right - rect.left;
	UINT height = rect.bottom - rect.top;

	if (!CreateDeviceD3D(hWnd,width,height))
	{
		CleanupDeviceD3D();
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("weaponIcons.ttf", 16.0f);

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
}

void Frame_Clear()
{
	if (g_mainRenderTargetView)
	{
		const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	}
}

void ImGui_Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupDeviceD3D();
}

void ImGui_NewFrame()
{
	Frame_Clear();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGui_Render()
{
	ImGui::ShowDemoWindow();
}

void ImGui_EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Vsync()
{
	HRESULT hr = g_pSwapChain->Present(1, 0);
	g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
}

void noVsync()
{
	HRESULT hr = g_pSwapChain->Present(0, 0);
	g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
}

void resize(UINT width, UINT height)
{

	if (g_pSwapChain)
	{
		g_pd3dDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		CleanupRenderTarget();
		g_pSwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_UNKNOWN, 0);
		CreateRenderTarget();
	}
}

void ImGui_MouseEvent()
{
	auto& io = ImGui::GetIO();
	io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON);
	io.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON);
}

void HandleToggle(std::function<void()> func) {
	static bool insert_pressed = false;

	if (GetAsyncKeyState(VK_DELETE)) {
		return;
	}

	if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
		if (!insert_pressed) {
			func();
			insert_pressed = true;
		}
	}
	else {
		insert_pressed = false;
	}
}