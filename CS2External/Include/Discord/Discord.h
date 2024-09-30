// Credit: https://github.com/SamuelTulach/OverlayCord
// Credit: https://github.com/TKazer/OS-DiscordImGui

#ifndef OVERLAYCORD_H
#define OVERLAYCORD_H

#include <Windows.h>
#include <functional> 
#include <string>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include <d3d11.h>
#include <dxgi.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

namespace Discord
{
	typedef struct _Header
	{
		UINT Magic;
		UINT FrameCount;
		UINT NoClue;
		UINT Width;
		UINT Height;
		BYTE Buffer[1];
	} Header;

	typedef struct _ConnectedProcessInfo
	{
		UINT ProcessId;
		HANDLE File;
		Header* MappedAddress;
	} ConnectedProcessInfo;

	inline bool ConnectToProcess(ConnectedProcessInfo& processInfo)
	{
		std::string mappedFilename = "DiscordOverlay_Framebuffer_Memory_" + std::to_string(processInfo.ProcessId);
		processInfo.File = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, mappedFilename.c_str());
		if (!processInfo.File || processInfo.File == INVALID_HANDLE_VALUE)
			return false;

		processInfo.MappedAddress = static_cast<Header*>(MapViewOfFile(processInfo.File, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		return processInfo.MappedAddress;
	}

	inline void DisconnectFromProcess(ConnectedProcessInfo& processInfo)
	{
		UnmapViewOfFile(processInfo.MappedAddress);
		processInfo.MappedAddress = nullptr;

		CloseHandle(processInfo.File);
		processInfo.File = nullptr;
	}

	inline void SendBuffer(Header*& pData, HANDLE& hMap, ID3D11DeviceContext* pd3dDeviceContext, IDXGISwapChain* pSwapChain, UINT& width, UINT& height)
	{
		if (!hMap)
			return;

		if (!pd3dDeviceContext || !pSwapChain)
			return;

		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;

		pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&pDevice));
		pDevice->GetImmediateContext(&pContext);

		ID3D11Texture2D* pBackBuffer;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));

		D3D11_TEXTURE2D_DESC TextureDesc;
		pBackBuffer->GetDesc(&TextureDesc);

		TextureDesc.BindFlags = 0;
		TextureDesc.Usage = D3D11_USAGE_STAGING;
		TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		ID3D11Texture2D* pBackBufferStaging;
		pDevice->CreateTexture2D(&TextureDesc, nullptr, &pBackBufferStaging);
		pContext->CopyResource(pBackBufferStaging, pBackBuffer);

		D3D11_MAPPED_SUBRESOURCE Resource;
		pd3dDeviceContext->Map(pBackBufferStaging, NULL, D3D11_MAP_READ, NULL, &Resource);

		pData->Width = width;
		pData->Height = height;
		int size = height * width * 4;

		memcpy_s(pData->Buffer, size, Resource.pData, size);

		pData->FrameCount++;
		pd3dDeviceContext->Unmap(pBackBufferStaging, 0);

		pBackBufferStaging->Release();
		pBackBuffer->Release();
	}
}
#endif