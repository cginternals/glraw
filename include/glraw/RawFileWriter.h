#pragma once

#include <glraw/glraw.h>
#include <glraw/RawDataHandlerInterface.h>

class QString;
class QDataStream;

namespace glraw
{

class AssetInformation;

class GLRAW_API RawFileWriter : public RawDataHandlerInterface
{
public:
    RawFileWriter();
    ~RawFileWriter();

    virtual bool write(AssetInformation & info, 
                       const std::function<void(QDataStream &)> & lambda);
    
    QString targetFilePath(const AssetInformation & info);

};

} // namespace glraw
