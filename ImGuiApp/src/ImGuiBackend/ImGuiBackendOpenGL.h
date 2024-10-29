#pragma once

#include <array>
#include "ImApp/ImGuiBackend/ImGuiBackend.h"

namespace ImApp
{
    class ImGuiBackendOpenGL: public ImGuiBackend
    {
    public:
        Backend GetBackendType() const override;
        void SetupDevice() override;
        void SetupImGui() override;
        void ClearImGui() override;
        void ClearDevice() override;
        void NewFrame() override;
        void Draw() override;
        void SetClearColor(float r, float g, float b, float a) override;
        void SwapBuffer() override;

    protected:
        void OnVSyncEnableSettle() override;

    private:
        std::array<float, 4> _clearColor = { 0.75f, 0.75f, 0.75f, 1.00f };

        // OpenGL
        void* _hDeviceHandle = nullptr;
        void* _hGLContext = nullptr;
    };
}
