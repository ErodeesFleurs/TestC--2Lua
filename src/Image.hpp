#pragma once

#include <string>
#include <stdexcept>

#include "stb_image.h"

class Image
{
public:
    Image(const std::string &path);
    ~Image();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
    unsigned char *getData() const { return data; }
    static std::string toHex();

private:
    int width, height, channels;
    unsigned char *data;
};
