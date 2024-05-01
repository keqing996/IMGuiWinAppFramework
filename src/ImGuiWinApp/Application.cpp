#include <locale>
#include "Application.h"

namespace ImGuiApp
{

    bool Application::Init()
    {
        // Language
        InitLanguage();

        // Main Window
        pMainWindow = CreateMainWindow();

        if (!pMainWindow->SetUp())
            return false;

        // Logic
        pLogic = CreateMainLogic();
        pLogic->SetUp(pMainWindow);

        pMainWindow->Show();

        return true;
    }

    void Application::Loop()
    {
        while (true)
        {
            bool isQuit = false;
            pMainWindow->WinMessageLoop(&isQuit);

            if (isQuit)
                break;

            pMainWindow->RenderLoop();
        }
    }

    void Application::Destroy()
    {
        pLogic->Destroy();
        pMainWindow->Destroy();

        delete pLogic;
        delete pMainWindow;
    }

    void Application::InitLanguage()
    {
        std::locale::global(std::locale("zh_CN.UTF8"));
    }

    std::string Application::GetFileLogPath()
    {
        return "log.txt";
    }
}