
#pragma once

#include <glraw/glraw.h>

#include <glraw/RawDataHandlerInterface.h>

class QString;

namespace glraw
{

class GLRAW_API AbstractFileWriter : public RawDataHandlerInterface
{
public:
    virtual ~AbstractFileWriter();

    virtual bool write(AssetInformation & info, 
                       const std::function<void(QDataStream &)> & lambda) = 0;

protected:
    static QString targetFilePath(const AssetInformation & info, const QString & fileExtension);

};

} // namespace glraw
