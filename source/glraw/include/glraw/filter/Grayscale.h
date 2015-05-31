#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API Grayscale : public AbstractFilter
{
	Grayscale();
	virtual ~Grayscale() = default;

	virtual bool process(Canvas & imageData, AssetInformation & info);

private:

};

}