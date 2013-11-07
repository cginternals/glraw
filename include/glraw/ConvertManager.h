
#pragma once

#include <glraw/glraw.h>

#include <QString>
#include <QLinkedList>
#include <glraw/RawConverter.h>
#include <glraw/WriterInterface.h>

namespace glraw
{

class ImageEditorInterface;

class GLRAW_API ConvertManager
{
public:
    ConvertManager(RawConverter & converter, WriterInterface & dataHandler);
    ~ConvertManager();

    bool process(const QString & inputFilePath);

    void appendImageEditor(ImageEditorInterface * editor);

protected:
    QLinkedList<ImageEditorInterface *> m_editors;
    RawConverter & m_converter;
    WriterInterface & m_dataHandler;

};

} // namespace glraw
