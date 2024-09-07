#include "color.h"
#include <cmath>

using namespace std;

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
}

float Color::Red() const { return r; }

float Color::Green() const { return g; }

float Color::Blue() const { return b; }

float Color::Alpha() const { return a; }

float Color::GetLuminance() const { return 0.2126*r + 0.7152*g + 0.0722*b; }

float Color::Clamp(float val, float min, float max){
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

Color Color::Clamp(float min, float max) const {
        float nr = abs(r), ng = abs(g), nb = abs(b), na = abs(a);
        if (nr < min) { nr = min; }
        if (ng < min) { ng = min; }
        if (nb < min) { nb = min; }
        if (na < min) { na = min; }

        if (nr > max) { nr = max; }
        if (ng > max) { ng = max; }
        if (nb > max) { nb = max; }
        if (na > max) { na = max; }
        return Color(nr, ng, nb, na);
}

Color Color::Opaque() const { return Color(r, g, b, 1); }

Color Color::operator*(float f) const { return Color(f*r, f*g, f*b, f*a); }

Color Color::operator*(Color c) const { return Color(c.r*r, c.g*g, c.b*b, c.a*a); }

Color Color::operator/(float f) const { return Color(r/f, g/f, b/f, a/f); }

Color Color::operator+(Color c) const { return Color(r + c.r, g + c.g, b + c.b, a + c.a); }

Color Color::operator+(float f) const { return Color(r + f, g + f, b + f, a + f); }

