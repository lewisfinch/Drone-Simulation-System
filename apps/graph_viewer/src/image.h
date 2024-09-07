#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>
#include "color.h"

class Image {
public:
    Image(int width, int height);
    void SaveAs(const std::string& filename) const;

    Color GetPixel(int x, int y) const;
    void SetPixel(int x, int y, Color color);
    void Clear(Color color);
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    void DrawLine(int startX, int startY, int endX, int endY, Color color);

private:
    int width;
    int height;
    unsigned char* pixels;
};

#endif // IMAGE_H_