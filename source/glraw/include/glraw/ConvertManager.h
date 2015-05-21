#pragma once

#include <QString>
#include <QScopedPointer>
#include <QLinkedList>

#include <glraw/glraw_api.h>

#include <glraw/Canvas.h>

namespace glraw
{

class ImageEditorInterface;
class AbstractConverter;
class AbstractFilter;
class AssetInformation;
class FileWriter;

class GLRAW_API ConvertManager
{
public:
    ConvertManager(
		FileWriter * writer = nullptr,
		AbstractConverter * converter = nullptr);

    ~ConvertManager();

	bool process(const QString & sourcePath);

	QByteArray convert(const QImage & image, AssetInformation & info);

	void appendImageEditor(ImageEditorInterface * editor);
	void appendFilter(AbstractFilter * filter);
    
	void setWriter(FileWriter * writer);
    void setConverter(AbstractConverter * converter);

protected:
    QLinkedList<ImageEditorInterface *> m_editors;
	QLinkedList<AbstractFilter*> m_filters;
    
    QScopedPointer<FileWriter> m_writer;
    QScopedPointer<AbstractConverter> m_converter;

	Canvas m_canvas;
};

} // namespace glraw
