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
    ImageFilterConfig.cpp \
    InputWindow.cpp \
    InputWindowField.cpp \
    InputWindowFieldBool.cpp \
    InputWindowFieldByte.cpp \
    InputWindowFieldInt.cpp \
    InputWindowFieldString.cpp \
    InputWindowFieldFloat.cpp

HEADERS += \
    MainWindow.hpp \
    FilterFastAdaptiveContrast.hpp \
    FilterInvert.hpp \
    Image.hpp \
    ImageFilter.hpp \
    lodepng.h \
    Pixel.hpp \
    Time.hpp \
    ImageView.hpp \
    ImageFilters.hpp \
    ImageFilterManager.hpp \
    ImageFilterConfig.hpp \
    InputWindow.hpp \
    InputWindowField.hpp \
    InputWindowFieldBool.hpp \
    InputWindowFieldByte.hpp \
    InputWindowFieldInt.hpp \
    InputWindowFieldString.hpp \
    InputWindowFieldFloat.hpp
