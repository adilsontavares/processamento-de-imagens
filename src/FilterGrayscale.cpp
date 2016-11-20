#include "FilterGrayscale.hpp"

FilterGrayscale::FilterGrayscale() : ImageFilter()
{
}

const char *FilterGrayscale::getName()
{
    return "Grayscale";
}

void FilterGrayscale::filterPixel(Image *image, unsigned int x, unsigned int y, Pixel& pixel)
{
    int gray = (pixel.r + pixel.g + pixel.b) / 3;

    pixel.r = gray;
    pixel.g = gray;
    pixel.b = gray;
}
