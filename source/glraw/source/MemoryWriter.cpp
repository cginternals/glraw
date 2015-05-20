
#include <glraw/MemoryWriter.h>

namespace glraw
{
    
MemoryWriter::MemoryWriter()
{
}

MemoryWriter::~MemoryWriter()
{
}

bool MemoryWriter::write( QByteArray && imageData, const QString & sourcePath, AssetInformation && info )
{
	mImageData = imageData;
	mInfo = info;

	return true;
}

const QByteArray& MemoryWriter::getImageData()
{
	return mImageData;
}

const AssetInformation& MemoryWriter::getAssetInformation()
{
	return mInfo;
}

} // namespace glraw
