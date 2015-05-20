
#include <glraw/ConvertManager.h>

#include <cassert>
#include <utility>

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QDataStream>

#include <glraw/AssetInformation.h>
#include <glraw/ImageEditorInterface.h>
#include <glraw/AbstractWriter.h>
#include <glraw/AbstractConverter.h>
#include <glraw/AbstractFilter.h>


namespace glraw
{

ConvertManager::ConvertManager(AbstractWriter * writer, AbstractConverter * converter)
:   m_writer(writer)
,   m_converter(converter)
{
}
    
ConvertManager::~ConvertManager()
{
	qDeleteAll(m_editors);
	qDeleteAll(m_filters);
}

bool ConvertManager::process(const QString & sourcePath)
{
	if (!QFile::exists(sourcePath))
	{
		qDebug() << "Input file does not exist.";
		return false;
	}

	//TODO input from QByteArray
	QImage image(sourcePath);

	return process(image);
}

bool ConvertManager::process(const QImage & image)
{
    assert(!m_converter.isNull());
    assert(!m_writer.isNull());

	if (image.isNull())
	{
		qDebug() << "Loading image from input file failed.";
		return false;
	}

    AssetInformation info;
    info.setProperty("width", image.width());
    info.setProperty("height", image.height());

	//TODO move to filters
    for (auto editor : m_editors)
        editor->editImage(image, info);

	m_canvas.loadTextureFromImage(image);

	for (auto filter : m_filters)
		filter->process(m_canvas, info);

    QByteArray imageData = m_converter->convert(m_canvas, info);

    if (imageData.isEmpty())
        return false;
    
	return m_writer->write( std::move( imageData ), sourcePath, std::move( info ) );
}

void ConvertManager::appendImageEditor(ImageEditorInterface * editor)
{
    m_editors.append(editor);
}
    
void ConvertManager::setWriter(AbstractWriter * writer)
{
    m_writer.reset(writer);
}
    
void ConvertManager::setConverter(AbstractConverter * converter)
{
    m_converter.reset(converter);
}

} // namespace glraw
