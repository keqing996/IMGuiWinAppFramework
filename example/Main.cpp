#include "ImApp/ImGuiWinApp.h"

int main()
{
    IMWinApp::ImGuiWinApp app(800, 600, "Hello world");

    app.AppLoop();

    return 0;
}