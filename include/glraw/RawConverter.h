
#pragma once

#include <glraw/glraw.h>

#include <assert.h>
#include <QImage>
#include <QDataStream>
#include <glraw/Enumerations.h>

namespace glraw
{
    
class AssetInformation;

class GLRAW_API RawConverter
{
public:
    RawConverter();
    ~RawConverter();

    void updateAssetInformation(AssetInformation & info);
    void convert(QImage & image, QDataStream & dataStream);

protected:
    template <typename Type>
    void write(const QImage & image, QDataStream & dataStream);

    Format m_format;
    Type m_type;

};

} // namespace glraw

#include <glraw/RawConverter.hpp>
