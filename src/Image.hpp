//
//  Image.hpp
//  FastAdaptiveContrast
//
//  Created by Adilson Tavares on 18/11/16.
//  Copyright © 2016 Adilson Tavares. All rights reserved.
//

#pragma once

#include "lodepng.h"
#include "Pixel.hpp"
#include "Time.hpp"

#include <iostream>
#include <vector>

class ImageFilter;

typedef std::vector<unsigned char> ImageData;

class Image
{
private:
    
    ImageData _data;
    unsigned int _width;
    unsigned int _height;
    
public:

    Image(ImageData data, unsigned int width, unsigned int height);
    
    static Image *load(const char *path);
    
    void applyFilter(ImageFilter *filter);
    
    unsigned int getWidth();
    unsigned int getHeight();
    
    ImageData getData();

    bool hasPixel(unsigned int x, unsigned int y);
    unsigned int indexForPixel(unsigned int x, unsigned int y);
    
    bool getPixel(unsigned int x, unsigned int y, Pixel *pixel);
    void setPixel(Pixel pixel, int x, int y);
    
    bool save(const char *path);
};
