#pragma once

#include "AbstractWriter.h"

namespace glraw
{

class GLRAW_API MemoryWriter : public AbstractWriter
{
public:
	MemoryWriter();
	virtual ~MemoryWriter();

    virtual bool write(
		QByteArray && imageData,
		const QString & sourcePath,
		AssetInformation && info) = 0;

private:

	QByteArray mImageData;
	AssetInformation mInto;
};

} // namespace glraw
