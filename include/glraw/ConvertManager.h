
#pragma once

#include <glraw/glraw.h>

#include <QString>
#include <QScopedPointer>
#include <QLinkedList>

namespace glraw
{

class ImageEditorInterface;
class FileWriter;
class RawConverter;

class GLRAW_API ConvertManager
{
public:
    ConvertManager(RawConverter * converter, FileWriter * writer);
    ~ConvertManager();

    bool process(const QString & sourcePath);

    void appendImageEditor(ImageEditorInterface * editor);

protected:
    QLinkedList<ImageEditorInterface *> m_editors;
    
    QScopedPointer<RawConverter> m_converter;
    QScopedPointer<FileWriter> m_writer;

};

} // namespace glraw
