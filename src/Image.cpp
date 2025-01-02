#include "Image.hpp"

Image::Image(const std::string &path)
{
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        throw std::runtime_error("Failed to load image: " + path);
    }
}

Image::~Image()
{
    stbi_image_free(data);
}