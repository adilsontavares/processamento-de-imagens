#pragma once

#include "ImageFilter.hpp"

class FilterGrayscale : public ImageFilter
{
public:

    FilterGrayscale();

    virtual const char *getName();
    virtual void filterPixel(Image *image, unsigned int x, unsigned int y, Pixel& pixel);
};
