
#pragma once

#include <glraw/glraw.h>
#include <glraw/AbstractFileWriter.h>

class QDataStream;

namespace glraw
{

class AssetInformation;

class GLRAW_API RawFileWriter : public AbstractFileWriter
{
public:
    RawFileWriter();
    ~RawFileWriter();

    virtual bool write(AssetInformation & info, 
                       const std::function<void(QDataStream &)> & lambda);

};

} // namespace glraw
