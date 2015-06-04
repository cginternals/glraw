
#include <glraw/ConvertManager.h>

#include <cassert>
#include <utility>

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QDataStream>

#include <glraw/AssetInformation.h>
#include <glraw/ImageEditorInterface.h>
#include <glraw/FileWriter.h>
#include <glraw/AbstractConverter.h>
#include <glraw/filter/AbstractFilter.h>


namespace glraw
{

ConvertManager::ConvertManager(FileWriter * writer, AbstractConverter * converter)
:   m_writer(writer)
,   m_converter(converter)
{
}
    
ConvertManager::~ConvertManager()
{
	//qDeleteAll(m_editors);
	qDeleteAll(m_filters);
}

bool ConvertManager::process(const QString & sourcePath)
{
	assert(!m_writer.isNull());

	if (!QFile::exists(sourcePath))
	{
		qDebug() << "Input file does not exist.";
		return false;
	}

	QImage image(sourcePath);
	AssetInformation info;

	QByteArray imageData = convert(image, info);

	if (imageData.isEmpty())
		return false;

	return m_writer->write(std::move(imageData), std::move(info), sourcePath);
}

QByteArray ConvertManager::convert(const QImage & image, AssetInformation & info)
{
	assert(!m_converter.isNull());

	//TODO input from QByteArray
	if (image.isNull())
	{
		qDebug() << "Loading image from input file failed.";
		return QByteArray();
	}

	info.setProperty("width", image.width());
	info.setProperty("height", image.height());

	//TODO move to filters
	//for (auto editor : m_editors)
	//   editor->editImage(image, info);

	m_canvas.loadTextureFromImage(image);

	//for (auto filter : m_filters)
	//	filter->process(m_canvas, info);

	return QByteArray();//m_converter->convert(m_canvas, info);
}

//void ConvertManager::appendImageEditor(ImageEditorInterface * editor)
//{
//    m_editors.append(editor);
//}
    
void ConvertManager::setWriter(FileWriter * writer)
{
    m_writer.reset(writer);
}
    
void ConvertManager::setConverter(AbstractConverter * converter)
{
    m_converter.reset(converter);
}

} // namespace glraw
