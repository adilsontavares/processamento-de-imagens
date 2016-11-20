#pragma once

class Math
{
private:
    Math();

public:

    static int clamp(int num, int min, int max);
    static float clamp(float num, float min, float max);
};
