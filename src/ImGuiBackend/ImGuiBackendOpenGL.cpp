#include "ImGuiBackendOpenGL.h"
#include "ImApp/Utility/WindowsInclude.h"
#include <backends/imgui_impl_opengl3.h>
#include <glad/gl.h>

namespace ImApp
{
    // https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
    void ImGuiBackendOpenGL::SetupDevice()
    {
        HDC hDeviceHandle = ::GetDC(static_cast<HWND>(_pWindow->GetSystemHandle()));

        ::gladLoaderLoadGL();

        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
            PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
            32,                   // Color depth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                   // Number of bits for the depth buffer
            8,                    // Number of bits for the stencil buffer
            0,                    // Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        int letWindowsChooseThisPixelFormat;
        letWindowsChooseThisPixelFormat = ::ChoosePixelFormat(hDeviceHandle, &pfd);
        ::SetPixelFormat(hDeviceHandle, letWindowsChooseThisPixelFormat, &pfd);

        _hGLContext = ::wglCreateContext(hDeviceHandle);
        ::wglMakeCurrent(hDeviceHandle, reinterpret_cast<HGLRC>(_hGLContext));
    }

    void ImGuiBackendOpenGL::ClearDevice()
    {
        if (_hGLContext)
        {
            HDC hDeviceHandle = ::GetDC(static_cast<HWND>(_pWindow->GetSystemHandle()));
            ::wglMakeCurrent(hDeviceHandle, nullptr);
            ::wglDeleteContext(static_cast<HGLRC>(_hGLContext));
        }
    }

    void ImGuiBackendOpenGL::SetupImGui()
    {
        ImGui_ImplOpenGL3_Init();
    }

    void ImGuiBackendOpenGL::ClearImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
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
        HDC hDeviceHandle = ::GetDC(static_cast<HWND>(_pWindow->GetSystemHandle()));
        ::SwapBuffers(hDeviceHandle);
    }

    void ImGuiBackendOpenGL::OnVSyncEnableSettle()
    {
    }
}
