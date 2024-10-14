#pragma once

#include <memory>
#include <ImApp/NativeWindow/Window.h>

#include "ImApp/Utility/Color.h"
#include "ImGuiBackendType.h"

namespace IMWinApp
{
    class ImGuiBackend
    {
    public:
        ImGuiBackend() = default;
        virtual ~ImGuiBackend() = default;

        static std::unique_ptr<ImGuiBackend> Create(NativeWindow::Window* pWindow, Backend backend);

    public:
        virtual void SetupDevice() = 0;
        virtual void SetupImGui() = 0;
        virtual void ClearImGui() = 0;
        virtual void ClearDevice() = 0;
        virtual void NewFrame() = 0;
        virtual void Draw() = 0;
        virtual void SwapBuffer() = 0;
        virtual void SetClearColor(const Utility::Color& color) = 0;

    public:
        void SetWindow(NativeWindow::Window* pWindow);
        void SetVSyncEnable(bool enable);
        bool GetVSyncEnable() const;

    protected:
        NativeWindow::Window* _pWindow;
        bool _enableVSync = true;
    };
}