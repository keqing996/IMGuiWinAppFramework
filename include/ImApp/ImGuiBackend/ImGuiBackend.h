#pragma once

#include <memory>

#include "ImGuiBackendType.h"

namespace IMWinApp
{
    class ImGuiBackend
    {
    public:
        virtual ~ImGuiBackend() = default;

        static std::unique_ptr<ImGuiBackend> Create(Backend backend);

        virtual void SetupDevice(void* windowHandle) = 0;
        virtual void SetupImGui() = 0;
        virtual void Clear() = 0;
        virtual void NewFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void Draw() = 0;

    private:

    };
}