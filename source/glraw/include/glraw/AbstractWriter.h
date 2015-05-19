#pragma once

class QByteArray;
class QString;


namespace glraw
{

class AssetInformation;

class GLRAW_API AbstractWriter
{
public:
	AbstractWriter() = default;
	virtual ~AbstractWriter() = default;

    virtual bool write(QByteArray && imageData, const QString & sourcePath, AssetInformation && info) = 0;
};

} // namespace glraw
