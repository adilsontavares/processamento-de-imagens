//
//  FilterFastAdaptiveContrast.hpp
//  FastAdaptiveContrast
//
//  Created by Adilson Tavares on 18/11/16.
//  Copyright © 2016 Adilson Tavares. All rights reserved.
//

#pragma once

#include "ImageFilter.hpp"

class FilterFastAdaptiveContrast : public ImageFilter
{
public:
    
    FilterFastAdaptiveContrast();
    
    virtual const char *getName();
    virtual void filterPixel(Pixel& pixel);
};
