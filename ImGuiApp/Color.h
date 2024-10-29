#pragma once

namespace Utility
{
    class Color
    {
        using uchar = unsigned char;

    public:
        Color();
        Color(int red, int green, int blue, int alpha = 255);
        Color(float red, float green, float blue, float alpha = 1.0f);

        uchar GetRed() const;
        uchar GetGreen() const;
        uchar GetBlue() const;
        uchar GetAlpha() const;

    public:
        static Color Red();
        static Color Green();
        static Color Blue();
        static Color Black();
        static Color White();
        static Color Magenta();
        static Color Cyan();
        static Color Yellow();
        static Color Gray();

    private:
        uchar _red;
        uchar _green;
        uchar _blue;
        uchar _alpha;
    };
}