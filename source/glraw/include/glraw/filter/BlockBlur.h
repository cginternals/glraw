#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API BlockBlur : public AbstractFilter
{
public:
	BlockBlur(unsigned int size);
	BlockBlur(const QVariantMap&);
	virtual ~BlockBlur() = default;

	int kernelSize() const;
	void setKernelSize(int size);

	virtual bool process(Canvas & imageData, AssetInformation & info);

private:

	int m_kernelSize;
};

}