
#include "ImApp/ImGuiBackend/ImGuiBackend.h"
#include "ImGuiBackendD3d11.h"
#include "ImGuiBackendOpenGL.h"
#include "ImGuiBackendVulkan.h"

namespace ImApp
{
    std::unique_ptr<ImGuiBackend> ImGuiBackend::Create(NativeWindow::Window* pWindow, Backend backend)
    {
        std::unique_ptr<ImGuiBackend> pResult;

        switch (backend)
        {
            case Backend::D3d11:
                pResult = std::make_unique<ImGuiBackendD3d11>();
                break;
            case Backend::OpenGL:
                pResult = std::make_unique<ImGuiBackendOpenGL>();
                break;
            case Backend::Vulkan:
                pResult = std::make_unique<ImGuiBackendVulkan>();
                break;
            default:
                pResult = std::make_unique<ImGuiBackendD3d11>();
                break;
        }

        pResult->SetWindow(pWindow);
        return pResult;
    }

    void ImGuiBackend::SetWindow(NativeWindow::Window* pWindow)
    {
        _pWindow = pWindow;
    }

    void ImGuiBackend::SetVSyncEnable(bool enable)
    {
        _enableVSync = enable;
    }

    bool ImGuiBackend::GetVSyncEnable() const
    {
        return _enableVSync;
    }
}
