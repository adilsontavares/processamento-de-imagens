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
#include <ctime>

#include <QObject>

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <sys/time.h>
#endif

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
#ifdef Q_OS_WIN

        SYSTEMTIME time;
        GetSystemTime(&time);

        double result = time.wSecond + time.wMilliseconds / 1000.0;
        return result;
#else
        struct timeval tv;
        gettimeofday(&tv, 0);
        return double(tv.tv_sec + tv.tv_usec / 1000000.0);
#endif
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
        sleepMilliseconds(uint32_t(seconds * 1000));
    }
};
