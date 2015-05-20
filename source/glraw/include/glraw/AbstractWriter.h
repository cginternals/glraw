#pragma once

#include <glraw/glraw_api.h>

class QString;
class QByteArray;

namespace glraw
{

class AssetInformation;

class GLRAW_API AbstractWriter
{
public:
	AbstractWriter() = default;
	virtual ~AbstractWriter() = default;

    virtual bool write(QByteArray && imageData, AssetInformation && info) = 0;
};

} // namespace glraw
