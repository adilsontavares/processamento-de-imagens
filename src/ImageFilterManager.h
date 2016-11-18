#pragma once

#include <iostream>
#include <vector>

#include "ImageFilter.hpp"

class ImageFilterManager
{
private:

	ImageFilterManager();

	std::vector<ImageFilter*> _filters;

public:

	static ImageFilterManager *instance();

	void addFilter(ImageFilter *filter);
	const std::vector<ImageFilter*>& getFilters();
};

