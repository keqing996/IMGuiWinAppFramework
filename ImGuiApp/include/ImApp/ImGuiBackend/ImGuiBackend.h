#pragma once

#include <memory>
#include <NativeWindow/Window.h>
#include "ImGuiBackendType.h"

namespace ImApp
{
    class ImGuiBackend
    {
    public:
        ImGuiBackend() = default;
        virtual ~ImGuiBackend() = default;

        static std::unique_ptr<ImGuiBackend> Create(NativeWindow::Window* pWindow, Backend backend);

    public:
        virtual Backend GetBackendType() const = 0;
        virtual void SetupDevice() = 0;
        virtual void SetupImGui() = 0;
        virtual void ClearImGui() = 0;
        virtual void ClearDevice() = 0;
        virtual void NewFrame() = 0;
        virtual void Draw() = 0;
        virtual void SwapBuffer() = 0;
        virtual void SetClearColor(float r, float g, float b, float a) = 0;

    protected:
        virtual void OnVSyncEnableSettle() = 0;

    public:
        void SetWindow(NativeWindow::Window* pWindow);
        void SetVSyncEnable(bool enable);
        bool GetVSyncEnable() const;

    protected:
        NativeWindow::Window* _pWindow;
        bool _enableVSync = true;
    };
}