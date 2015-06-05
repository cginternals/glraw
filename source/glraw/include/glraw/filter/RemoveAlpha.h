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

	virtual bool process( std::unique_ptr<Canvas> & imageData, AssetInformation & info ) override;
};

}
