//
//  ImageFilter.hpp
//  FastAdaptiveContrast
//
//  Created by Adilson Tavares on 18/11/16.
//  Copyright © 2016 Adilson Tavares. All rights reserved.
//

#ifndef __IMAGE_FILTER__
#define __IMAGE_FILTER__

#include "Image.hpp"

class ImageFilter
{
protected:
    
    ImageFilter();
    
    virtual void filterPixel(Image *image, unsigned int x, unsigned int y, Pixel& pixel);
    
public:
    
    virtual bool configure();

    virtual const char *getName() = 0;
    virtual void apply(Image *image);
};

#endif
