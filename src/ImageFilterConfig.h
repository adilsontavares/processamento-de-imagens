#pragma once

#include "ImageFilterManager.h"

class ImageFilterConfig
{
private:

	ImageFilterConfig();

	static void configure(ImageFilterManager *manager);

public:

	static void configure();
};

