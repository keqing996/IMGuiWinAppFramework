#include "ImGuiBackendOpenGL.h"
#include <backends/imgui_impl_opengl3.h>
#include <glad/gl.h>

namespace IMWinApp
{
    void ImGuiBackendOpenGL::SetupDevice()
    {
        _pWindow->CreateOpenGLContext();
    }

    void ImGuiBackendOpenGL::SetupImGui()
    {
        ImGui_ImplOpenGL3_Init();
    }

    void ImGuiBackendOpenGL::ClearImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
    }

    void ImGuiBackendOpenGL::ClearDevice()
    {
        _pWindow->ReleaseOpenGLContext();
    }

    void ImGuiBackendOpenGL::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void ImGuiBackendOpenGL::Draw()
    {
        auto [width, height] = _pWindow->GetSize();
        ::glViewport(0, 0, width, height);
        ::glClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
        ::glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiBackendOpenGL::SetClearColor(const Utility::Color& color)
    {
        _clearColor[0] = color.GetRed();
        _clearColor[1] = color.GetGreen();
        _clearColor[2] = color.GetBlue();
        _clearColor[3] = color.GetAlpha();
    }

    void ImGuiBackendOpenGL::SwapBuffer()
    {
        _pWindow->SwapBuffer();
    }
}
