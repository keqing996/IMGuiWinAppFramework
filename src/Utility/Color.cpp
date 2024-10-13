
#include <algorithm>
#include "ImApp/Utility/Color.h"

namespace Utility
{
    Color::Color(uchar red, uchar green, uchar blue, uchar alpha)
        : _red(red)
        , _green(green)
        , _blue(blue)
        , _alpha(alpha)
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
}
