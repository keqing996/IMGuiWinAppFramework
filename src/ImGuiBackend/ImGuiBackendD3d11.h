#pragma once

#include <d3d11.h>
#include <array>
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
        void Draw() override;
        void SetClearColor(const Utility::Color& color) override;
        void SwapBuffer() override;

    private:
        std::array<float, 4> _clearColor;

        ID3D11Device* _pD3dDevice = nullptr;
        ID3D11DeviceContext* _pD3dDeviceContext = nullptr;
        IDXGISwapChain* _pSwapChain = nullptr;
        ID3D11RenderTargetView* _pMainRenderTargetView = nullptr;
    };
}
