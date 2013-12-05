
#pragma once

#include <glraw/glraw.h>

#include <QString>
#include <QScopedPointer>
#include <QLinkedList>

namespace glraw
{

class ImageEditorInterface;
class AbstractFileWriter;
class RawConverter;

class GLRAW_API ConvertManager
{
public:
    ConvertManager(RawConverter * converter, AbstractFileWriter * writer);
    ~ConvertManager();

    bool process(const QString & inputFilePath);

    void appendImageEditor(ImageEditorInterface * editor);
    void setWriter(AbstractFileWriter * writer);

protected:
    QLinkedList<ImageEditorInterface *> m_editors;
    
    QScopedPointer<RawConverter> m_converter;
    QScopedPointer<AbstractFileWriter> m_writer;

};

} // namespace glraw
