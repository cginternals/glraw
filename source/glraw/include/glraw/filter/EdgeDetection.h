#pragma once
#include <glraw/filter/AbstractKernel.h>

namespace glraw
{

class GLRAW_API EdgeDetection : public AbstractKernel
{
public:
	EdgeDetection(unsigned int size, float factor);
	EdgeDetection(const QVariantMap& cfg);
	virtual ~EdgeDetection() = default;

protected:
	virtual QString firstShader() const override;
	virtual QString secondShader() const override;
};

}