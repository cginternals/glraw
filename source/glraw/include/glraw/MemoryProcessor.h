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
	virtual ~MemoryProcessor();

	bool process(QByteArray & data, AssetInformation & info);

	void appendFilter(AbstractFilter * filter);
	void setConverter(AbstractConverter * converter);

	void reset();

protected:

	bool applyFilter(AssetInformation & info);
	bool copyImageFromGL(QByteArray & data, AssetInformation & info);

	QLinkedList<AbstractFilter*> m_filters;
	QScopedPointer<AbstractConverter> m_converter;

	std::unique_ptr<Canvas> & canvas();

private:

	std::unique_ptr<Canvas> m_canvas;
};

} // namespace glraw
