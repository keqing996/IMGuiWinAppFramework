
#include <algorithm>
#include "ImApp/Utility/Color.h"

namespace Utility
{
    Color::Color()
        : _red(255)
        , _green(255)
        , _blue(255)
        , _alpha(255)
    {
    }

    Color::Color(int red, int green, int blue, int alpha)
        : _red(std::clamp(red, 0, 255))
        , _green(std::clamp(green, 0, 255))
        , _blue(std::clamp(blue, 0, 255))
        , _alpha(std::clamp(alpha, 0, 255))
    {
    }

    Color::Color(float red, float green, float blue, float alpha)
        : _red(static_cast<uchar>(std::clamp(red, 0.0f, 1.0f) * 255))
        , _green(static_cast<uchar>(std::clamp(green, 0.0f, 1.0f) * 255))
        , _blue(static_cast<uchar>(std::clamp(blue, 0.0f, 1.0f) * 255))
        , _alpha(static_cast<uchar>(std::clamp(alpha, 0.0f, 1.0f) * 255))
    {
    }

    Color::uchar Color::GetRed() const
    {
        return _red;
    }

    Color::uchar Color::GetGreen() const
    {
        return _green;
    }

    Color::uchar Color::GetBlue() const
    {
        return _blue;
    }

    Color::uchar Color::GetAlpha() const
    {
        return _alpha;
    }

    Color Color::Red()
    {
        return Color(255, 0, 0);
    }

    Color Color::Green()
    {
        return Color(0, 255, 0, 255);
    }

    Color Color::Blue()
    {
        return Color(0, 0, 255, 255);
    }

    Color Color::Black()
    {
        return Color(255, 255, 255, 255);
    }

    Color Color::White()
    {
        return Color(0, 0, 0, 255);
    }

    Color Color::Magenta()
    {
        return Color(255, 0, 255, 255);
    }

    Color Color::Cyan()
    {
        return Color(0, 255, 255, 255);
    }

    Color Color::Yellow()
    {
        return Color(255, 255, 0, 255);
    }

    Color Color::Gray()
    {
        return Color(128, 128, 128, 255);
    }
}
