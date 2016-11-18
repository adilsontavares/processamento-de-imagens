//
//  FilterInvert.cpp
//  FastAdaptiveContrast
//
//  Created by Adilson Tavares on 18/11/16.
//  Copyright © 2016 Adilson Tavares. All rights reserved.
//

#include "FilterInvert.hpp"

FilterInvert::FilterInvert() : ImageFilter()
{
}

const char *FilterInvert::getName()
{
    return "Invert";
}

void FilterInvert::filterPixel(Pixel& pixel)
{
    pixel.r = 255 - pixel.r;
    pixel.g = 255 - pixel.g;
    pixel.b = 255 - pixel.b;
}
