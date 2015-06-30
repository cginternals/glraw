#pragma once

#include <QString>

#include <glraw/glraw_api.h>

#include <glraw/MemoryProcessor.h>
#include <glraw/AssetInformation.h>
#include <glraw/FileWriter.h>

namespace glraw
{

class GLRAW_API ConvertManager : public MemoryProcessor
{
public:
    ConvertManager(FileWriter * writer = nullptr, AbstractConverter * converter = nullptr);
    virtual ~ConvertManager() = default;

	bool process(const QString & sourcePath);
    
	void setWriter(FileWriter * writer);

protected:
    
	static AssetInformation generateAssetInformation(const QImage & image);

    QScopedPointer<FileWriter> m_writer;
};

} // namespace glraw
