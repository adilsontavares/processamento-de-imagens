//
//  Time.hpp
//  FastAdaptiveContrast
//
//  Created by Adilson Tavares on 17/11/16.
//  Copyright © 2016 Adilson Tavares. All rights reserved.
//

#pragma once

#include <cstdlib>
#include <thread>
#include <chrono>
#include <sys/time.h>

class Time
{
private:
    
    Time();
    
public:
    
    static uint32_t milliseconds()
    {
        return uint32_t(seconds() * 1000.0);
    }
    
    static double seconds()
    {
        struct timeval tv;
        gettimeofday(&tv, 0);
        return double(tv.tv_sec + tv.tv_usec / 1000000.0);
    }
    
    static void sleepNanoseconds(uint32_t nanoseconds)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
    }
    
    static void sleepMilliseconds(uint32_t milliseconds)
    {
        sleepNanoseconds(milliseconds * 1000000);
    }
    
    static void sleepSeconds(double seconds)
    {
        sleepMilliseconds(seconds * 1000);
    }
};
