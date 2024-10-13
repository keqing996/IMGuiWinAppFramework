#pragma once

#include <d3d11.h>
#include "ImApp/ImGuiBackend/ImGuiBackend.h"

namespace IMWinApp
{
    class ImGuiBackendD3d11: public ImGuiBackend
    {
    public:
        void SetupDevice(void* windowHandle) override;
        void SetupImGui() override;
        void Clear() override;
        void NewFrame() override;
        void EndFrame() override;
        void Draw() override;

    private:
        void D3d11Clear();
        bool ImGuiSetUp();
        void ImGuiClear();

    private:
        ID3D11Device* _pD3dDevice = nullptr;
        ID3D11DeviceContext* _pD3dDeviceContext = nullptr;
        IDXGISwapChain* _pSwapChain = nullptr;
        ID3D11RenderTargetView* _pMainRenderTargetView = nullptr;
    };
}