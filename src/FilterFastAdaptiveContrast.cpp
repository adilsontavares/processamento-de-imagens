//
//  FilterFastAdaptiveContrast.cpp
//  FastAdaptiveContrast
//
//  Created by Adilson Tavares on 18/11/16.
//  Copyright © 2016 Adilson Tavares. All rights reserved.
//

#include "FilterFastAdaptiveContrast.hpp"


FilterFastAdaptiveContrast::FilterFastAdaptiveContrast() : ImageFilter()
{
}

const char *FilterFastAdaptiveContrast::getName()
{
    return "Fast Adaptive Contrast";
}

void FilterFastAdaptiveContrast::filterPixel(Pixel& pixel)
{
    pixel.r = 0;
}
