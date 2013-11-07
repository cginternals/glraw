
#pragma once

#include <glraw/glraw.h>

#include <functional>

class QDataStream;

namespace glraw
{
    
class AssetInformation;

class GLRAW_API WriterInterface
{
public:
    virtual ~WriterInterface() {};

    virtual bool write(AssetInformation & info, 
                       const std::function<void(QDataStream &)> & lambda) = 0;

};

} // namespace glraw
