#ifndef COLOR_H_
#define COLOR_H_

#include <string>

class Color {
public:
    Color(float r, float g, float b, float a);

    float Red() const;
    float Green() const;
    float Blue() const;
    float Alpha() const;

    float GetLuminance() const;
    Color Clamp(float min, float max) const;
    static float Clamp(float val, float min, float max);
    Color Opaque() const;

    Color operator*(float f) const;
    Color operator/(float f) const;
    Color operator*(Color c) const;
    Color operator+(Color c) const;
    Color operator+(float f) const;

private:
    float r,g,b,a;
};

#endif // COLOR_H_