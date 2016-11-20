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
private:

    float _c;

    float function(int x, float alpha, unsigned char w);

public:
    
    FilterFastAdaptiveContrast();

    virtual bool configure();
    
    virtual const char *getName();
    virtual void apply(Image *image);
};
