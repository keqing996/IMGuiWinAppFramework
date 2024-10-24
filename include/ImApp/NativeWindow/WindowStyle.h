#pragma once

namespace NativeWindow
{
    class WindowStyle
    {
    public:
        enum class Type
        {
            Popup,
            Overlapped
        };

        struct PopupStyle
        {
            // nothing
        };

        struct OverlappedStyle
        {
            bool haveTitleBar = true;
            bool haveResize = true;
            bool haveClose = true;
        };

        union Config
        {
            PopupStyle popup;
            OverlappedStyle overlapped;
        };

    public:
        Type type;
        Config config;
    };
}