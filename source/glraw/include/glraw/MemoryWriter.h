#pragma once

#include <glraw/AbstractWriter.h>
#include <glraw/AssetInformation.h>

#include <QString>

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
		AssetInformation && info);

private:

	QByteArray mImageData;
	AssetInformation mInfo;
};

} // namespace glraw
