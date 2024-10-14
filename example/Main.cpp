#include "ImApp/ImGuiWinApp.h"

class DemoWIndow: public IMWinApp::ImGuiWinApp
{

};

int main()
{
    DemoWIndow app;
    app.InitWindow(800, 600, "Hello world");
    app.Loop();

    return 0;
}