#include "ImGuiBackendD3d11.h"
#include <backends/imgui_impl_dx11.h>

#pragma comment(lib, "d3d11.lib")

namespace ImApp
{
    void ImGuiBackendD3d11::SetupDevice()
    {
        HWND hWnd = static_cast<HWND>(_pWindow->GetSystemHandle());

        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        HRESULT res = ::D3D11CreateDeviceAndSwapChain(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                createDeviceFlags,
                featureLevelArray,
                2,
                D3D11_SDK_VERSION,
                &sd,
                &_pSwapChain,
                &_pD3dDevice,
                &featureLevel,
                &_pD3dDeviceContext
                );

        if (res != S_OK)
            return;

        // Render target
        ID3D11Texture2D* pBackBuffer;
        _pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        _pD3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pMainRenderTargetView);
        pBackBuffer->Release();
    }

    void ImGuiBackendD3d11::SetupImGui()
    {
        ImGui_ImplDX11_Init(_pD3dDevice, _pD3dDeviceContext);
    }

    void ImGuiBackendD3d11::ClearImGui()
    {
        ImGui_ImplDX11_Shutdown();
    }

    void ImGuiBackendD3d11::ClearDevice()
    {
        if (_pMainRenderTargetView)
        {
            _pMainRenderTargetView->Release();
            _pMainRenderTargetView = nullptr;
        }

        if (_pSwapChain)
        {
            _pSwapChain->Release();
            _pSwapChain = nullptr;
        }

        if (_pD3dDeviceContext)
        {
            _pD3dDeviceContext->Release();
            _pD3dDeviceContext = nullptr;
        }

        if (_pD3dDevice)
        {
            _pD3dDevice->Release();
            _pD3dDevice = nullptr;
        }
    }

    void ImGuiBackendD3d11::NewFrame()
    {
        ImGui_ImplDX11_NewFrame();
    }

    void ImGuiBackendD3d11::Draw()
    {
        _pD3dDeviceContext->OMSetRenderTargets(1, &_pMainRenderTargetView, nullptr);
        _pD3dDeviceContext->ClearRenderTargetView(_pMainRenderTargetView, _clearColor.data());

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiBackendD3d11::SetClearColor(const Utility::Color& color)
    {
        _clearColor[0] = color.GetRed();
        _clearColor[1] = color.GetGreen();
        _clearColor[2] = color.GetBlue();
        _clearColor[3] = color.GetAlpha();
    }

    void ImGuiBackendD3d11::SwapBuffer()
    {
        if (_enableVSync)
            _pSwapChain->Present(1, 0);
        else
            _pSwapChain->Present(0, 0);
    }

    void ImGuiBackendD3d11::OnVSyncEnableSettle()
    {
        // do nothing
    }
}
