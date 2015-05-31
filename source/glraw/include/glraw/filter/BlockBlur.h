#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API BlockBlur : public AbstractFilter
{
	BlockBlur(unsigned int size);
	virtual ~BlockBlur() = default;

	int getKernelSize() const;
	void setKernelSize(int size);

	virtual bool process(Canvas & imageData, AssetInformation & info);

private:

	int m_kernelSize;
};

}