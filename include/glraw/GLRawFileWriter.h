#pragma once

#include <cstdint>

#include <glraw/glraw.h>

#include <glraw/AbstractFileWriter.h>
#include <glraw/GLRawFile.h>

#include <QVariant>
#include <QMap>

class QFile;

namespace glraw
{

class GLRAW_API GLRawFileWriter : public AbstractFileWriter
{
public:
    GLRawFileWriter();
    ~GLRawFileWriter();

    virtual bool write(AssetInformation & info, const std::function<void(QDataStream &)> & lambda);

protected:
    void writeHeader(QDataStream & dataStream, QFile & file, AssetInformation & info);

    unsigned typeIndicator(QVariant::Type type);
    void writeValue(QDataStream & dataStream, const QVariant & value);
    void writeString(QDataStream & dataStream, const QString & string);

    static QMap<QVariant::Type, int> s_typeIndicators;

};

} // namespace glraw
