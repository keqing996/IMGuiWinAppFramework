#pragma once

#include <string>
#include <vector>
#include <d3d11.h>
#include "Util.hpp"
#include "IWinMsgReceiver.h"
#include "ImGuiRender.h"
#include "ImGuiLogic.h"

namespace ImGuiApp
{
    class Win32Window: NonCopyable
    {
    public:
        Win32Window() = default;
        virtual ~Win32Window() = default;

    public:
        virtual bool SetUp();
        virtual void Destroy();
        void Show();
        void WinMessageLoop(bool* isQuit);
        void RenderLoop();
        void AddWinMsgProc(IWinMsgReceiver* pWinMsgReceiver);
        void RemoveWinMsgProc(IWinMsgReceiver* pWinMsgReceiver);
        void AddImGuiLogicUpdater(ImGuiLogic* pUpdater);
        void RemoveImGuiLogicUpdater(ImGuiLogic* pUpdater);

    public: /* getter */
        HWND GetWindowHandle() const;
        ID3D11Device* GetD3dDevice() const;
        ID3D11DeviceContext* GetD3dDeviceContext() const;
        ImGuiRender* GetRender() const;

    protected: /* init para */

        /**
         * @brief Get win32 window register name.
         */
        virtual const char* GetWindowRegisterName();

        /**
         * @brief Get window title in utf-8.
         */
        virtual const char* GetWindowTitle() = 0;

        /**
         * @brief Window init width.
         */
        virtual int GetWindowInitWidth() = 0;

        /**
         * @brief Window init height.
         */
        virtual int GetWindowInitHeight() = 0;

        /**
         * @brief Does window have max box.
         */
        virtual bool WindowHasMaxBox();

        /**
         * @brief Does window have min box.
         */
        virtual bool WindowHasMinBox();

        /**
         * @brief Can window be resized by user.
         */
        virtual bool WindowCanThickFrame();

        /**
         * @brief Window's icon id in rc file.
         */
        virtual int GetWindowIconResourceId();

        /**
         * @brief Does window update render when no message received.
         * @note If return true, main loop will use GetMessage to let thread sleep,
         * reduce cpu usage. If return false, use PeekMessage to keep main loop always running.
         */
        virtual bool BlockWhenNoWindowsMessage();

    private: /* message */
        static LRESULT WINAPI WndProcDispatch(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        LRESULT OnWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgSysCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private: /* render */
        void ClearColor();
        void SwapChain();

    private:
        void Win32RegisterWindow();
        void Win32CreateWindow();
        void Win32DestroyWindow();
        void Win32UnRegisterWindow();
        bool D3dCreateDevice();
        void D3dDestroyDevice();
        void D3dCreateRenderTarget();
        void D3dDestroyRenderTarget();
        void ImGuiCreateRender();
        void ImGuiDestroyRender();

    private:
        /* Basic */
        int _width;
        int _height;
        HWND _hWnd;
        std::wstring _windowRegisterName;
        std::wstring _windowTitle;

        /* Msg Receive */
        std::vector<IWinMsgReceiver*> _winMsgReceiverVec;

        /* D3D */
        ID3D11Device* _pD3dDevice = nullptr;
        ID3D11DeviceContext* _pD3dDeviceContext = nullptr;
        IDXGISwapChain* _pSwapChain = nullptr;
        ID3D11RenderTargetView* _pMainRenderTarget = nullptr;

        /* ImGuiRender */
        ImGuiRender* _pImGuiRender = nullptr;

        /* ImGuiLogic */
        std::vector<ImGuiLogic*> _imGuiLogicVec;
    };
}

