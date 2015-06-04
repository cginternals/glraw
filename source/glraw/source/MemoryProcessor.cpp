
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
	qDeleteAll(m_filters);
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

	if (!m_canvas)
	{
		m_canvas = std::unique_ptr<Canvas>(new Canvas());
	}

	m_canvas->loadTexture(data, info);

	for (auto filter : m_filters)
	{
		if (!filter->process(m_canvas, info))
		{
			qCritical() << "ERROR!!";
			return false;
		}
	}

	data = m_converter->convert(m_canvas, info);
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

} // namespace glraw
