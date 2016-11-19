#include "ImageFilterManager.hpp"

ImageFilterManager::ImageFilterManager()
{
}

ImageFilterManager * ImageFilterManager::instance()
{
	static ImageFilterManager *instance = 0;
	if (!instance)
		instance = new ImageFilterManager();
	return instance;
}

void ImageFilterManager::addFilter(ImageFilter * filter)
{
	_filters.push_back(filter);
}

const std::vector<ImageFilter*>& ImageFilterManager::getFilters()
{
	return _filters;
}
