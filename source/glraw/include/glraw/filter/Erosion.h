#pragma once
#include <glraw/filter/AbstractKernel.h>

namespace glraw
{
class GLRAW_API Erosion : public AbstractKernel
{
public:
	Erosion(unsigned int size, float factor);
	Erosion(const QVariantMap& cfg);
	virtual ~Erosion() = default;

protected:
	virtual QString firstShader() const override;
	virtual QString secondShader() const override;
};

}