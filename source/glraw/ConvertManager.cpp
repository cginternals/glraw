
#include <glraw/ConvertManager.h>

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QDataStream>
#include <glraw/AssetInformation.h>
#include <glraw/ImageEditorInterface.h>

namespace glraw
{

ConvertManager::ConvertManager(RawConverter & converter, RawDataHandlerInterface & dataHandler)
:   m_converter(converter)
,   m_dataHandler(dataHandler)
{
}
    
ConvertManager::~ConvertManager()
{
}

bool ConvertManager::process(const QString & inputFilePath)
{
    if (!QFile::exists(inputFilePath))
    {
        qDebug() << "Input file does not exist.";
        return false;
    }
    
    QImage image(inputFilePath);
    if (image.isNull())
    {
        qDebug() << "Loading image from input file failed.";
        return false;
    }

    AssetInformation info;
    info.setProperty("inputFilePath", inputFilePath);
    info.setProperty("width", image.width());
    info.setProperty("height", image.height());

    for (auto editor : m_editors)
        editor->editImage(image, info);

    m_converter.updateAssetInformation(info);

    m_dataHandler.write(info, [this, &image] (QDataStream & dataStream) {
        m_converter.convert(image, dataStream);
    });

    return true;
}

void ConvertManager::appendImageEditor(ImageEditorInterface * editor)
{
    m_editors.append(editor);
}

} // namespace glraw
