#include "FilterFastAdaptiveContrast.hpp"
#include "InputWindow.hpp"
#include "FilterGrayscale.hpp"

#include "Math.hpp"
#include <cmath>

FilterFastAdaptiveContrast::FilterFastAdaptiveContrast() : ImageFilter()
{
}

const char *FilterFastAdaptiveContrast::getName()
{
    return "Fast Adaptive Contrast";
}

bool FilterFastAdaptiveContrast::configure()
{
    auto window = new InputWindow();
    window->addField("C", "Conductivity Factor", InputWindowField::Type::Float);

    if (!window->exec())
        return false;

    _c = window->getFloat("C");

    return true;
}

float FilterFastAdaptiveContrast::function(int x, float alpha, unsigned char w)
{
    auto a = alpha / (2.0 * w);
    auto b = (alpha / w) * x - alpha - 1.0;
    auto c = (alpha / (2.0 * w)) * x * x - alpha * x + x;

    return (-b - sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
}

void FilterFastAdaptiveContrast::apply(Image *image)
{
    const unsigned int count = image->getWidth() * image->getHeight();

    auto pixels = new unsigned char[count];
    auto avg = new unsigned char[count];
    auto min = new unsigned char[count];
    auto max = new unsigned char[count];
    auto out = new unsigned char[count];

    Pixel pixel;

    // Buscar por todos os pixels e copia para um vetor temporário
    for (unsigned int x = 0; x < image->getWidth(); ++x)
    {
        for (unsigned y = 0; y < image->getHeight(); ++y)
        {
            image->getPixel(x, y, &pixel);

            auto index = image->indexForPixel(x, y);
            auto value = ((int)pixel.r + pixel.g + pixel.b) / 3;

            pixels[index] = value;
            avg[index] = value;
            min[index] = value;
            max[index] = value;
            out[index] = value;
        }
    }

    // Linhas
    for (unsigned int x = 0; x < image->getWidth(); ++x)
    {
        for (unsigned y = 0; y < image->getHeight(); ++y)
        {
            auto current = image->indexForPixel(x, y);
            auto neighbor = image->indexForPixel(x - 1, y);

            if (neighbor >= 0)
            {
                avg[current] = (1.0 - _c) * avg[current] + _c * avg[neighbor];

                if (min[neighbor] < min[current])
                    min[current] = (1 - _c) * min[current] + _c * min[neighbor];

                if (max[neighbor] > max[current])
                    max[current] = (1 - _c) * max[current] + _c * max[neighbor];
            }

            neighbor = image->indexForPixel(x, y - 1);

            if (neighbor >= 0)
            {
                avg[current] = (1.0 - _c) * avg[current] + _c * avg[neighbor];

                if (min[neighbor] < min[current])
                    min[current] = (1 - _c) * min[current] + _c * min[neighbor];

                if (max[neighbor] > max[current])
                    max[current] = (1 - _c) * max[current] + _c * max[neighbor];
            }

            auto w = max[current] - min[current];
            auto diff = max[current] - min[current];

            if (diff > 0 && min[current] <= pixels[current] && avg[current] <= max[current])
            {
                auto iNew = w * ((pixels[current] - min[current]) / diff);
                auto aNew = w * ((avg[current] - min[current]) / diff);

                auto alpha = (aNew - iNew) / 128.0;
                auto f = function(iNew, alpha, w);
                auto result = min[current] + f;

                out[current] = (unsigned char)Math::clamp((int)result, 0, 255);
            }
        }
    }

    // Salva o resultado na imagem
    for (unsigned int x = 0; x < image->getWidth(); ++x)
    {
        for (unsigned y = 0; y < image->getHeight(); ++y)
        {
            auto index = image->indexForPixel(x, y);
            auto value = out[index];

            pixel.r = value;
            pixel.g = value;
            pixel.b = value;
            pixel.a = 255;

            image->setPixel(pixel, x, y);
        }
    }
}
