
#include <glraw/ConvertManager.h>

#include <cassert>

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

ConvertManager::ConvertManager(FileWriter * writer, AbstractConverter * converter)
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
    
    if (!QFile::exists(sourcePath))
    {
        qDebug() << "Input file does not exist.";
        return false;
    }
    
    QImage image(sourcePath);
    if (image.isNull())
    {
        qDebug() << "Loading image from input file failed.";
        return false;
    }

    AssetInformation info;
    info.setProperty("width", image.width());
    info.setProperty("height", image.height());

    for (auto editor : m_editors)
        editor->editImage(image, info);

    QByteArray imageData = m_converter->convert(image, info);
    
    if (imageData.isEmpty())
        return false;
    
    m_writer->write(imageData, sourcePath, info);

    return true;
}

void ConvertManager::appendImageEditor(ImageEditorInterface * editor)
{
    m_editors.append(editor);
}
    
void ConvertManager::setWriter(FileWriter * writer)
{
    m_writer.reset(writer);
}
    
void ConvertManager::setConverter(AbstractConverter * converter)
{
    m_converter.reset(converter);
}

} // namespace glraw
