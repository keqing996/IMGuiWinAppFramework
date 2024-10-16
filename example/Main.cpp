#include <format>
#include "ImApp/ImGuiWinApp.h"
#include "ImApp/Component/Window.h"
#include "ImApp/Component/SameLine.h"

using namespace ImApp;

class DemoWindow: public ImGuiWinApp
{
protected:
    void Tick() override
    {
        ImGui::PushFont(_pFontRegularNormal);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {6, 12});
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 6});
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);

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

        Window("Hello, world!", window_flags, []()
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            SameLine({
                []() -> void {
                    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                        counter++;
                },
                []() -> void {
                    if (ImGui::SmallButton("SmallButton"))
                        counter++;
                },
                []() -> void {
                    if (ImGui::ArrowButton("ArrowButton", ImGuiDir_Up))
                        counter++;
                },
                []() -> void {
                    ImGui::Text("counter = %d", counter);
                }
            });

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::SeparatorText("SeparatorText");

            ImGui::Bullet();
            ImGui::Text("Bullet"); // auto same line

            if (ImGui::RadioButton("RadioButton", false))
                counter++;
        });

        ImGui::PopStyleVar(3);
        ImGui::PopFont();
    }
};

int main()
{
    DemoWindow app;
    app.InitWindow(800, 600, "Hello world", NativeWindow::DefaultWindowStyle, ImApp::Backend::OpenGL);
    app.Loop();

    return 0;
}