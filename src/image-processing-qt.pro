QT += widgets core gui

CONFIG += c++11

TARGET = image-processing-qt
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp \
    FilterFastAdaptiveContrast.cpp \
    FilterInvert.cpp \
    Image.cpp \
    ImageFilter.cpp \
    lodepng.cpp \
    ImageView.cpp \
    ImageFilterManager.cpp \
    ImageFilterConfig.cpp

HEADERS += \
    MainWindow.hpp \
    FilterFastAdaptiveContrast.hpp \
    FilterInvert.hpp \
    Image.hpp \
    ImageFilter.hpp \
    ImageFilters.h \
    lodepng.h \
    Pixel.hpp \
    Time.hpp \
    ImageView.h \
    ImageFilterManager.h \
    ImageFilterConfig.h
