
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
}

bool ConvertManager::process(const QString & sourcePath)
{
    assert(!m_converter.isNull());
    assert(!m_writer.isNull());
    
	//TODO fix loading error for "source".
    if (!QFile::exists(sourcePath))
    {
        qDebug() << "Input file does not exist.";
        return false;
    }
    
	//TODO input from memory & filesystem + virtual
    QImage image(sourcePath);
    if (image.isNull())
    {
        qDebug() << "Loading image from input file failed.";
        return false;
    }

    AssetInformation info;
    info.setProperty("width", image.width());
    info.setProperty("height", image.height());

	//TODO hardwarebeschleunigung
    for (auto editor : m_editors)
        editor->editImage(image, info);

	//TODO multi + gl context nur einmal.
	m_canvas.loadTextureFromImage(image);
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
