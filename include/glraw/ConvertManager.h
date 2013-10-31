
#pragma once

#include <glraw/glraw.h>

#include <QString>
#include <QLinkedList>
#include <glraw/RawConverter.h>
#include <glraw/RawDataHandlerInterface.h>

namespace glraw
{

class ImageEditorInterface;

class GLRAW_API ConvertManager
{
public:
    ConvertManager(RawConverter & converter, RawDataHandlerInterface & dataHandler);
    ~ConvertManager();

    bool process(const QString & inputFilePath);

    void appendImageEditor(ImageEditorInterface * editor);

protected:
    QLinkedList<ImageEditorInterface *> m_editors;
    RawConverter & m_converter;
    RawDataHandlerInterface & m_dataHandler;

};

} // namespace glraw
