#include "ImApp/ImGuiWinApp.h"
#include "ImApp/ImGuiComponent.h"

class DemoWIndow: public IMWinApp::ImGuiWinApp
{
protected:
    void Tick() override
    {
        ImGuiIO& io = ImGui::GetIO();

        static float f = 0.0f;
        static int counter = 0;

        const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(mainViewport->WorkSize, ImGuiCond_Always);

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoCollapse;

        ImGui::Begin("Hello, world!", nullptr, window_flags);

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
};

int main()
{
    DemoWIndow app;
    app.InitWindow(800, 600, "Hello world", NativeWindow::DefaultWindowStyle, IMWinApp::Backend::OpenGL);
    app.Loop();

    return 0;
}