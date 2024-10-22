#include "ImGuiBackendOpenGL.h"
#include "ImApp/Utility/WindowsInclude.h"
#include <backends/imgui_impl_opengl3.h>
#include <glad/gl.h>
#include <glad/wgl.h>

#pragma comment(lib, "opengl32.lib")

namespace ImApp
{
    // https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
    void ImGuiBackendOpenGL::SetupDevice()
    {
        _hDeviceHandle = ::GetDC(static_cast<HWND>(_pWindow->GetSystemHandle()));

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

        int letWindowsChooseThisPixelFormat = ::ChoosePixelFormat(static_cast<HDC>(_hDeviceHandle), &pfd);
        ::SetPixelFormat(static_cast<HDC>(_hDeviceHandle), letWindowsChooseThisPixelFormat, &pfd);

        _hGLContext = ::wglCreateContext(static_cast<HDC>(_hDeviceHandle));
        ::wglMakeCurrent(static_cast<HDC>(_hDeviceHandle), static_cast<HGLRC>(_hGLContext));

        // Loading GL function must after gl context make current.
        ::gladLoaderLoadGL();
        ::gladLoaderLoadWGL(static_cast<HDC>(_hDeviceHandle)); // for wgl extension function

        // refresh vsync
        OnVSyncEnableSettle();
    }

    void ImGuiBackendOpenGL::ClearDevice()
    {
        HWND hWnd = static_cast<HWND>(_pWindow->GetSystemHandle());

        if (_hDeviceHandle)
        {
            ::wglMakeCurrent(static_cast<HDC>(_hDeviceHandle), nullptr);
            ::ReleaseDC(hWnd, static_cast<HDC>(_hDeviceHandle));
            _hDeviceHandle = nullptr;
        }

        if (_hGLContext)
        {
            ::wglDeleteContext(static_cast<HGLRC>(_hGLContext));
            _hGLContext = nullptr;
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
        if (_hDeviceHandle)
            ::SwapBuffers(static_cast<HDC>(_hDeviceHandle));
    }

    void ImGuiBackendOpenGL::OnVSyncEnableSettle()
    {
        // wglSwapInterval is from WGL_EXT_swap_control, so maybe null
        if (wglSwapIntervalEXT != nullptr)
            ::wglSwapIntervalEXT(_enableVSync);
    }
}
