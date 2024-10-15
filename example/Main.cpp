#include <format>
#include "ImApp/ImGuiWinApp.h"
#include "ImApp/Component/Window.h"
#include "ImApp/Component/Text.h"
#include "ImApp/Component/Button.h"
#include "ImApp/Component/HorizontalLayout.h"
#include "imgui.h"

using namespace IMWinApp;

class DemoWindow: public ImGuiWinApp
{
public:
    DemoWindow(): window("Hello, world!")
    {
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoCollapse;

        window.SetWindowFlags(window_flags);

        auto pText1 = new Text("This is some useful text.");

        auto pButton = new Button("Button", [this](){ counter++; });
        pCounterText = new Text("counter");

        auto pHorizontal = new HorizontalLayout();
        pHorizontal->AddComponent(pButton);
        pHorizontal->AddComponent(pCounterText);

        pFrameText = new Text("frame");

        window.AddComponent(pText1).AddComponent(pHorizontal).AddComponent(pFrameText);
    }

    Window window;
    Text* pCounterText = nullptr;
    Text* pFrameText = nullptr;
    int counter = 0;


protected:
    void Tick() override
    {
        ImGuiIO& io = ImGui::GetIO();
        pCounterText->SetText(std::format("counter = {}", counter));
        pFrameText->SetText(std::format("Application average {:.3f} ms/frame ({:.1f} FPS)", 1000.0f / io.Framerate, io.Framerate));
        /*
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
        */

        window.UpdateView();
    }
};

int main()
{
    DemoWindow app;
    app.InitWindow(800, 600, "Hello world", NativeWindow::DefaultWindowStyle, IMWinApp::Backend::OpenGL);
    app.Loop();

    return 0;
}