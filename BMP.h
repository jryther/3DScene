/*
*BMP Reader by Brandon
*Source: https://stackoverflow.com/questions/20595340/loading-a-tga-bmp-file-in-c-opengl
*/


#pragma once
#include <vector>
#include <fstream>

#ifdef __APPLE__
#include <GL/glut.h>
#endif


#ifdef _WIN32
#include <GL/glut.h>
#endif


typedef union PixelInfo
{
    std::uint32_t Colour;
    struct
    {
        std::uint8_t B, G, R, A;
    };
} *PPixelInfo;


class BMP
{
private:
    std::uint32_t width, height;
    std::uint16_t BitsPerPixel;
    std::vector<std::uint8_t> Pixels;

public:
    BMP(const char* FilePath);
    std::vector<std::uint8_t> GetPixels() const { return this->Pixels; }
    std::uint32_t GetWidth() const { return this->width; }
    std::uint32_t GetHeight() const { return this->height; }
    bool HasAlphaChannel() { return BitsPerPixel == 32; }
};

