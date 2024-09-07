#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <cmath>
#include <iostream>

Image::Image(int width, int height): width(width), height(height) {
    pixels = new unsigned char[width*height*4];
}

void Image::SaveAs(const std::string& filename) const {
    stbi_write_png(filename.c_str(), width, height, 4, pixels, width*4);
}

Color Image::GetPixel(int x, int y) const {
    if (x < 0) { x = 0; }
    if (x >= width) { x = width - 1; }
    if (y < 0) { y = 0; }
    if (y >= height) { y = height - 1; }

    unsigned char* pixel = &pixels[(x + width*y)*4];
    Color c(
            1.0* pixel[0] / 255.0, // red
            1.0* pixel[1] / 255.0, // green
            1.0* pixel[2] / 255.0, // blue
            1.0* pixel[3] / 255.0 // alpha);
         );
    return c;
}

void Image::SetPixel(int x, int y, Color color) {
    unsigned char* pixel = &pixels[(x + width*y)*4];
    pixel[0] = color.Red() *255.0;
    pixel[1] = color.Green() *255.0;
    pixel[2] = color.Blue() *255.0;
    pixel[3] = color.Alpha() *255.0;
}

void Image::Clear(Color color) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            SetPixel(x, y, color);
        }
    }
}

void Image::DrawLine(int startX, int startY, int endX, int endY, Color color) {
    float dist = std::sqrt((endX-startX)*(endX-startX) + (endY-startY)*(endY-startY));
    float dx = 1.0*(endX-startX)/dist;
    float dy = 1.0*(endY-startY)/dist;

    float x = startX;
    float y = startY;

    for (int i = 0; i < dist; i++) {
        SetPixel(x, y, color);
        x += dx;
        y += dy;
    }
}