//
//  Image.cpp
//  FastAdaptiveContrast
//
//  Created by Adilson Tavares on 18/11/16.
//  Copyright © 2016 Adilson Tavares. All rights reserved.
//

#include "Image.hpp"
#include "ImageFilter.hpp"

#include <cassert>

Image::Image(ImageData data, unsigned int width, unsigned int height)
{
    _data = data;
    _width = width;
    _height = height;

    assert(data.size() == (width * height * 4));
}

Image *Image::load(const char *path)
{
    ImageData png;
    ImageData data;
    
    unsigned int error;
    unsigned int width;
    unsigned int height;
    
    error = lodepng::load_file(png, path);
    if (error)
        return 0;
    
    error = lodepng::decode(data, width, height, png);
    if (error)
        return 0;
    
    auto image = new Image(data, width, height);
    return image;
}

bool Image::hasPixel(unsigned int x, unsigned int y)
{
    auto valid = x < _width && y < _height;
    return valid;
}

unsigned int Image::indexForPixel(unsigned int x, unsigned int y)
{
    if (!hasPixel(x, y))
        return -1;

    return (x * 4) + (y * _width * 4);
}

void Image::applyFilter(ImageFilter *filter)
{
    filter->apply(this);
}

unsigned int Image::getWidth()
{
    return _width;
}

unsigned int Image::getHeight()
{
    return _height;
}

const ImageData& Image::getData()
{
    return _data;
}

void Image::setPixel(Pixel pixel, int x, int y)
{
    if (!hasPixel(x, y))
        return;
    
    auto index = indexForPixel(x, y);
    
    _data[index    ] = pixel.r;
    _data[index + 1] = pixel.g;
    _data[index + 2] = pixel.b;
    _data[index + 3] = pixel.a;
}

bool Image::getPixel(unsigned int x, unsigned int y, Pixel *pixel)
{
    if (!hasPixel(x, y))
    {
        pixel = 0;
        return false;
    }
    
    auto output = Pixel();
    auto index = indexForPixel(x, y);
    
    output.r = _data[index    ];
    output.g = _data[index + 1];
    output.b = _data[index + 2];
    output.a = _data[index + 3];
    
    *pixel = output;
    
    return true;
}

bool Image::save(const char *path)
{
    ImageData output;
    
    if (lodepng::encode(output, _data, _width, _height) || lodepng::save_file(output, path))
        return false;
    
    return true;
}
