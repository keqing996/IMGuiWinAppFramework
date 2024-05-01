#pragma once

namespace ImGuiApp
{
    class IWinMsgReceiver
    {
    public:
        virtual void OnWinMsg(int64_t hWnd, uint32_t msg, int64_t wParam, int64_t lParam) = 0;
    };
}