#pragma once

#include "stb_image.h"
#include <string>
#include <stdexcept>

class Image
{
public:
    Image(const std::string &path);
    ~Image();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
    unsigned char *getData() const { return data; }

private:
    int width, height, channels;
    unsigned char *data;
};