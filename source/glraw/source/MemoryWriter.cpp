
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


} // namespace glraw
