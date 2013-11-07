#pragma once

#include <glraw/glraw.h>

#include <glraw/AbstractFileWriter.h>

class QFile;

namespace glraw
{

class GLRAW_API GLRawFileWriter : public AbstractFileWriter
{
public:
    static char s_magicNumber[4];

    GLRawFileWriter();
    ~GLRawFileWriter();

    virtual bool write(AssetInformation & info, 
                       const std::function<void(QDataStream &)> & lambda);

protected:
    void writeHeader(QDataStream & dataStream,
                     QFile & file,
                     AssetInformation & info);

protected:

};

} // namespace glraw
