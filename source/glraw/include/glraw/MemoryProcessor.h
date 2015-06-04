#pragma once

#include <memory>

#include <QScopedPointer>
#include <QLinkedList>

#include <glraw/glraw_api.h>


namespace glraw
{

class AbstractConverter;
class AbstractFilter;
class AssetInformation;
class Canvas;

class GLRAW_API MemoryProcessor
{
public:
	MemoryProcessor(AbstractConverter * converter = nullptr);
	~MemoryProcessor();

	bool process(QByteArray & data, AssetInformation & info);

	void appendFilter(AbstractFilter * filter);
	void setConverter(AbstractConverter * converter);

protected:

	QLinkedList<AbstractFilter*> m_filters;
	QScopedPointer<AbstractConverter> m_converter;

	std::unique_ptr<Canvas> m_canvas;
};

} // namespace glraw
