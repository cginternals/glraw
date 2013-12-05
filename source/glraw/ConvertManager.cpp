
#include <glraw/ConvertManager.h>

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QDataStream>

#include <glraw/AssetInformation.h>
#include <glraw/ImageEditorInterface.h>
#include <glraw/AbstractFileWriter.h>
#include <glraw/RawConverter.h>

namespace glraw
{

ConvertManager::ConvertManager(RawConverter * converter, AbstractFileWriter * writer)
:   m_converter(converter)
,   m_writer(writer)
{
}
    
ConvertManager::~ConvertManager()
{
    qDeleteAll(m_editors);
}

bool ConvertManager::process(const QString & sourcePath)
{
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
    m_writer->write(imageData, sourcePath, info);

    return true;
}

void ConvertManager::appendImageEditor(ImageEditorInterface * editor)
{
    m_editors.append(editor);
}

void ConvertManager::setWriter(AbstractFileWriter * writer)
{
    m_writer.reset(writer);
}

} // namespace glraw
