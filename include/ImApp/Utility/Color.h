#pragma once

namespace Utility
{
    class Color
    {
        using uchar = unsigned char;

    public:
        Color(uchar red, uchar green, uchar blue, uchar alpha = 255);
        Color(float red, float green, float blue, float alpha = 1.0f);

        uchar GetRed() const;
        uchar GetGreen() const;
        uchar GetBlue() const;
        uchar GetAlpha() const;

    private:
        uchar _red;
        uchar _green;
        uchar _blue;
        uchar _alpha;
    };
}