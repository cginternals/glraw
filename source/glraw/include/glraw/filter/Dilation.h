#pragma once
#include <glraw/filter/AbstractKernel.h>

namespace glraw
{

class GLRAW_API Dilation : public AbstractKernel
{
public:
	Dilation(unsigned int size, float factor);
	Dilation(const QVariantMap& cfg);
	virtual ~Dilation() = default;

protected:
	virtual QString firstShader() const override;
	virtual QString secondShader() const override;
};

}