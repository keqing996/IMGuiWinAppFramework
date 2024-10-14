#include "ImApp/ImGuiWinApp.h"

class DemoWIndow: public IMWinApp::ImGuiWinApp
{

};

int main()
{
    DemoWIndow app;
    app.InitWindow(800, 600, "Hello world", NativeWindow::DefaultWindowStyle, IMWinApp::Backend::OpenGL);
    app.Loop();

    return 0;
}