#pragma once

#include "ImageFilterManager.hpp"

class ImageFilterConfig
{
private:

	ImageFilterConfig();

	static void configure(ImageFilterManager *manager);

public:

	static void configure();
};

