#include "Math.hpp"

#include <cmath>
#include <algorithm>

Math::Math()
{

}

int Math::clamp(int num, int min, int max)
{
    return std::min(max, std::max(min, num));
}

float Math::clamp(float num, float min, float max)
{
    return std::min(max, std::max(min, num));
}
