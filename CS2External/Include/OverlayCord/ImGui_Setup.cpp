#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>
#include <d3d11.h>
#include "Discord.h"
#include <vector>

#pragma comment(lib,"d3d11.lib")

static std::vector<uint8_t>		m_frame;
static D3D11_TEXTURE2D_DESC     desc					= {};
static D3D11_MAPPED_SUBRESOURCE mappedResource			= {};
static ID3D11Device*			g_pd3dDevice			= nullptr;
static ID3D11DeviceContext*		g_pd3dDeviceContext		= nullptr;
static IDXGISwapChain*			g_pSwapChain			= nullptr;
static bool                     g_SwapChainOccluded		= false;
static ID3D11RenderTargetView*	g_mainRenderTargetView	= nullptr;
static ID3D11Texture2D*			m_renderTargetTexture	= nullptr;
static ID3D11Texture2D*			m_stagingTexture		= nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void DX11_Setup(UINT& width, UINT& height)
{

	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &g_pd3dDevice, nullptr, &g_pd3dDeviceContext);
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET;

	g_pd3dDevice->CreateTexture2D(&desc, nullptr, &m_renderTargetTexture);

	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;

}

void ImGui_Setup(HWND hWnd)
{
	if (!CreateDeviceD3D(hWnd))
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

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
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
	const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, color);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGui_Render()
{
	ImGui::Begin("Hello, world!");
	ImGui::End();
}

void ImGui_EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

std::vector<uint8_t> Discord_Draw(UINT& width, UINT& height)
{
	m_frame.resize(width * height * 4);
	g_pd3dDeviceContext->CopyResource(m_stagingTexture, m_renderTargetTexture);
	g_pd3dDeviceContext->Map(m_stagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
	memcpy(m_frame.data() + sizeof(Discord::_Header), mappedResource.pData, m_frame.size());
	g_pd3dDeviceContext->Unmap(m_stagingTexture, 0);

	auto& header = *reinterpret_cast<Discord::_Header*>(m_frame.data());
	//header.Magic = Discord::_Header::Magic;
	header.FrameCount++;
	header.Width = width;
	header.Height = height;

	return m_frame;
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