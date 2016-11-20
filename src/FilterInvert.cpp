#include "FilterInvert.hpp"

FilterInvert::FilterInvert() : ImageFilter()
{
}

const char *FilterInvert::getName()
{
    return "Invert";
}

void FilterInvert::filterPixel(Image *image, unsigned int x, unsigned int y, Pixel& pixel)
{
    pixel.r = 255 - pixel.r;
    pixel.g = 255 - pixel.g;
    pixel.b = 255 - pixel.b;
}
