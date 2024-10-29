#pragma once

#include <d3d11.h>
#include <array>
#include "ImApp/ImGuiBackend/ImGuiBackend.h"

namespace ImApp
{
    class ImGuiBackendD3d11: public ImGuiBackend
    {
    public:
        Backend GetBackendType() const override;
        void SetupDevice() override;
        void SetupImGui() override;
        void ClearImGui() override;
        void ClearDevice() override;
        void NewFrame() override;
        void Draw() override;
        void SetClearColor(const Utility::Color& color) override;
        void SwapBuffer() override;

    protected:
        void OnVSyncEnableSettle() override;

    private:
        std::array<float, 4> _clearColor = { 0.75f, 0.75f, 0.75f, 1.00f };

        ID3D11Device* _pD3dDevice = nullptr;
        ID3D11DeviceContext* _pD3dDeviceContext = nullptr;
        IDXGISwapChain* _pSwapChain = nullptr;
        ID3D11RenderTargetView* _pMainRenderTargetView = nullptr;
    };
}
