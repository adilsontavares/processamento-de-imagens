#pragma once

#include "ImageFilter.hpp"

class FilterInvert : public ImageFilter
{
public:
    
    FilterInvert();
    
    virtual const char *getName();
    virtual void filterPixel(Image *image, unsigned int x, unsigned int y, Pixel& pixel);
};
