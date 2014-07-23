#pragma once

#include <glraw/glraw_api.h>

#include <QString>
#include <QScopedPointer>
#include <QLinkedList>


namespace glraw
{

class ImageEditorInterface;
class FileWriter;
class AbstractConverter;

class GLRAW_API ConvertManager
{
public:
    ConvertManager(FileWriter * writer = nullptr,
                   AbstractConverter * converter = nullptr);
    ~ConvertManager();

    bool process(const QString & sourcePath);

    void appendImageEditor(ImageEditorInterface * editor);
    
    void setWriter(FileWriter * writer);
    void setConverter(AbstractConverter * converter);

protected:
    QLinkedList<ImageEditorInterface *> m_editors;
    
    QScopedPointer<FileWriter> m_writer;
    QScopedPointer<AbstractConverter> m_converter;

};

} // namespace glraw
