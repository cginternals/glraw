#pragma once
#include <glraw/filter/AbstractKernel.h>

namespace glraw
{

class GLRAW_API BlockBlur : public AbstractKernel
{
public:
	BlockBlur(unsigned int size, float factor);
	BlockBlur(const QVariantMap&);
	virtual ~BlockBlur() = default;

protected:
	virtual QString firstShader() const override;
	virtual QString secondShader() const override;

};

}