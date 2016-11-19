#include "ImageFilterConfig.hpp"

#include "FilterInvert.hpp"
#include "FilterFastAdaptiveContrast.hpp"

ImageFilterConfig::ImageFilterConfig()
{
}

void ImageFilterConfig::configure()
{
	configure(ImageFilterManager::instance());
}

void ImageFilterConfig::configure(ImageFilterManager *manager)
{
	manager->addFilter(new FilterInvert());
	manager->addFilter(new FilterFastAdaptiveContrast());
}