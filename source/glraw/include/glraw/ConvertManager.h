#pragma once

#include <QString>
#include <QScopedPointer>
#include <QLinkedList>

#include <glraw/glraw_api.h>

#include <glraw/Canvas.h>

namespace glraw
{

class ImageEditorInterface;
class AbstractWriter;
class AbstractConverter;
class AbstractFilter;

class GLRAW_API ConvertManager
{
public:
    ConvertManager(
		AbstractWriter * writer = nullptr,
		AbstractConverter * converter = nullptr);

    ~ConvertManager();

    bool process(const QString & sourcePath);

	void appendImageEditor(ImageEditorInterface * editor);
	void appendFilter(AbstractFilter * filter);
    
	void setWriter(AbstractWriter * writer);
    void setConverter(AbstractConverter * converter);

protected:
    QLinkedList<ImageEditorInterface *> m_editors;
	QLinkedList<AbstractFilter*> m_filters;
    
    QScopedPointer<AbstractWriter> m_writer;
    QScopedPointer<AbstractConverter> m_converter;

	Canvas m_canvas;
};

} // namespace glraw
