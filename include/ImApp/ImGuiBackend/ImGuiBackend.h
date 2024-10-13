#pragma once

#include <memory>

#include "ImApp/Utility/Color.h"
#include "ImGuiBackendType.h"

namespace IMWinApp
{
    class ImGuiBackend
    {
    public:
        ImGuiBackend() = default;
        virtual ~ImGuiBackend() = default;

        static std::unique_ptr<ImGuiBackend> Create(Backend backend);

    public:
        virtual void SetupDevice(void* windowHandle) = 0;
        virtual void SetupImGui() = 0;
        virtual void Clear() = 0;
        virtual void NewFrame() = 0;
        virtual void Draw() = 0;
        virtual void SwapBuffer(bool enableVsync) = 0;
        virtual void SetClearColor(const Utility::Color& color) = 0;
    };
}