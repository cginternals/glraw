
#include <glraw/MemoryProcessor.h>

#include <cassert>
#include <utility>

#include <QDebug>
#include <QImage>
#include <QDataStream>

#include <glraw/AssetInformation.h>
#include <glraw/AbstractConverter.h>
#include <glraw/filter/AbstractFilter.h>
#include <glraw/Canvas.h>

namespace glraw
{

MemoryProcessor::MemoryProcessor(AbstractConverter * converter)
	: m_converter(converter)
{
}

MemoryProcessor::~MemoryProcessor()
{
	reset();
}

bool MemoryProcessor::process(QByteArray & data, AssetInformation & info)
{
	assert(!m_converter.isNull());
	assert(info.propertyExists("width"));
	assert(info.propertyExists("height"));

	if (data.isNull())
	{
		qCritical("Input image data is null.");
		return false;
	}

	canvas()->loadTexture(data, info);

	if(!applyFilter(info))
	{
		return false;
	}

	return copyImageFromGL(data, info);
}

bool MemoryProcessor::applyFilter(AssetInformation & info)
{
	for(auto filter : m_filters)
	{
		if(!filter->process(canvas(), info))
		{
			qCritical() << "ERROR!!";
			return false;
		}
	}
	return true;
}

bool MemoryProcessor::copyImageFromGL(QByteArray & data, AssetInformation & info)
{
	data = m_converter->convert(canvas(), info);
	return !data.isEmpty();
}

void MemoryProcessor::setConverter(AbstractConverter * converter)
{
	m_converter.reset(converter);
}

void MemoryProcessor::appendFilter(AbstractFilter * filter)
{
	assert(filter);
	m_filters.append(filter);
}

std::unique_ptr<Canvas> & MemoryProcessor::canvas()
{
	if(!m_canvas)
	{
		m_canvas = std::unique_ptr<Canvas>(new Canvas());
	}

	return m_canvas;
}

void MemoryProcessor::reset()
{
	qDeleteAll(m_filters);
}

} // namespace glraw
