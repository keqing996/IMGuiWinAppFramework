
#include "ImApp/ImGuiBackend/ImGuiBackend.h"
#include "ImGuiBackendD3d11.h"
#include "ImGuiBackendOpenGL.h"
#include "ImGuiBackendVulkan.h"

namespace IMWinApp
{
    std::unique_ptr<ImGuiBackend> ImGuiBackend::Create(Backend backend)
    {
        switch (backend)
        {
            case Backend::D3d11:
                return std::make_unique<ImGuiBackendD3d11>();
            case Backend::OpenGL:
            case Backend::Vulkan:
            default:
                return std::make_unique<ImGuiBackendD3d11>();
        }
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
