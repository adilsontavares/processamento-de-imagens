//
//  ImageFilter.cpp
//  FastAdaptiveContrast
//
//  Created by Adilson Tavares on 18/11/16.
//  Copyright © 2016 Adilson Tavares. All rights reserved.
//

#include "ImageFilter.hpp"

ImageFilter::ImageFilter()
{
}

void ImageFilter::apply(Image *image)
{
    Pixel pixel;
    
    for (unsigned int x = 0; x < image->getWidth(); ++x)
    {
        for (unsigned int y = 0; y < image->getHeight(); ++y)
        {
            image->getPixel(x, y, &pixel);
            filterPixel(pixel);
            image->setPixel(pixel, x, y);
        }
    }
}

void ImageFilter::filterPixel(Pixel& pixel)
{
    pixel.r = 255;
    pixel.g = 0;
    pixel.b = 255;
}
