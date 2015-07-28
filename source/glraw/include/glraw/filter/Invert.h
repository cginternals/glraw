#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

class GLRAW_API Invert : public AbstractFilter
{
public:
	Invert() = default;
	Invert( const QVariantMap& cfg );
	virtual ~Invert() = default;

protected:
	virtual QString fragmentShaderSource(unsigned int pass) override;
};

}