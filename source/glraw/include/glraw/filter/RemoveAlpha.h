#pragma once
#include <glraw/filter/AbstractFilter.h>

#include <QVariantMap>

namespace glraw
{

class GLRAW_API RemoveAlpha : public AbstractFilter
{
public:
	RemoveAlpha() = default;
	RemoveAlpha(const QVariantMap& in);
	virtual ~RemoveAlpha() = default;

protected:

	virtual QString fragmentShaderSource(unsigned int pass) override;

};

}
