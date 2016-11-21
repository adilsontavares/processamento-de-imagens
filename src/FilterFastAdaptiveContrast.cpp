#include "FilterFastAdaptiveContrast.hpp"
#include "InputWindow.hpp"
#include "FilterGrayscale.hpp"

#include "Math.hpp"
#include <cmath>
#include <cassert>

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
    window->addField("W0", "Limiar de ruído", InputWindowField::Type::Byte);

    if (!window->exec())
        return false;

    _c = window->getFloat("C");
    _w0 = window->getByte("W0");

    return true;
}

float FilterFastAdaptiveContrast::function(int x, float alpha, int w)
{
    auto a = alpha / (2.0 * w);
    auto b = (alpha / w) * x - alpha - 1.0;
    auto c = (alpha / (2.0 * w)) * x * x - alpha * x + x;

    return (-b - sqrtf(b * b - 4.0 * a * c)) / (2.0 * a);
}

void FilterFastAdaptiveContrast::apply(Image *image)
{
    const unsigned int count = image->getWidth() * image->getHeight();

    auto pixels = new int[count];
    auto avg = new float[count];
    auto min = new float[count];
    auto max = new float[count];
    auto out = new int[count];

    Pixel pixel;

    // Buscar por todos os pixels e copia para um vetor temporário, transformando em tons de cinza
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

    // Calcula a média nas linhas
    for (unsigned y = 0; y < image->getHeight(); ++y)
    {
        for (unsigned int x = 0; x < image->getWidth(); ++x)
        {
            auto current = image->indexForPixel(x, y);
            auto neighbor = image->indexForPixel(x - 1, y);

            if (neighbor >= 0)
            {
                avg[current] = (1.0 - _c) * avg[current] + _c * avg[neighbor];

                if (min[neighbor] < min[current])
                    min[current] = (1.0 - _c) * min[current] + _c * min[neighbor];

                if (max[neighbor] > max[current])
                    max[current] = (1.0 - _c) * max[current] + _c * max[neighbor];
            }
        }
    }

    // Calcula a média nas colunas
    for (unsigned int x = 0; x < image->getWidth(); ++x)
    {
        for (unsigned y = 0; y < image->getHeight(); ++y)
        {
            auto current = image->indexForPixel(x, y);
            auto neighbor = image->indexForPixel(x, y - 1);

            if (neighbor >= 0)
            {
                avg[current] = (1.0 - _c) * avg[current] + _c * avg[neighbor];

                if (min[neighbor] < min[current])
                    min[current] = (1.0 - _c) * min[current] + _c * min[neighbor];

                if (max[neighbor] > max[current])
                    max[current] = (1.0 - _c) * max[current] + _c * max[neighbor];
            }
        }
    }

    // Calcula o resultado
    for (unsigned int x = 0; x < image->getWidth(); ++x)
    {
        for (unsigned y = 0; y < image->getHeight(); ++y)
        {
            auto current = image->indexForPixel(x, y);

            auto lmin = min[current];
            auto lmax = max[current];

            if (min[current] <= pixels[current] && avg[current] <= max[current])
            {
                auto diff = lmax - lmin;
                auto w = diff;

                if ((lmax - lmin) == 0)
                    continue;

                auto iNew = w * ((pixels[current] - lmin) / diff);
                auto aNew = w * ((   avg[current] - lmin) / diff);
                auto alpha = (aNew - iNew) / 128.0;

                auto f = function(iNew, alpha, w);
                auto result = lmin + f;

                if (std::isnan(f))
                    continue;

                out[current] = Math::clamp(result, 0.0, 255.0);
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
