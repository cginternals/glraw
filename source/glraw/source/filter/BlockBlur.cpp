#include <glraw/filter/BlockBlur.h>

#include <cassert>

namespace glraw
{

BlockBlur::BlockBlur(unsigned int size)
{
	setKernelSize(size);
}

bool BlockBlur::process(Canvas & imageData, AssetInformation & info)
{
	return true;
}

int BlockBlur::getKernelSize() const
{
	return m_kernelSize;
}

void BlockBlur::setKernelSize(int size)
{
	assert(size > 1);
	assert(size % 2 == 1);

	m_kernelSize = size;
}

}