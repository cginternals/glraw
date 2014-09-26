#pragma once

#include <cstdint>

#include <QtGui/qopengl.h>
#include <QMap>
#include <QVariant>

#include <glraw/glraw_api.h>

#include <glraw/RawFile.h>


class QFile;
class QDataStream;

namespace glraw
{

class AssetInformation;

class GLRAW_API FileWriter
{
public:
    FileWriter(bool headerEnabled = true, bool suffixesEnabled = true);
    virtual ~FileWriter();

    virtual bool write(const QByteArray & imageData,
                       const QString & sourcePath, 
                       AssetInformation & info);

    bool headerEnabled() const;
    void setHeaderEnabled(bool b);

    bool suffixesEnabled() const;
    void setSuffixesEnabled(bool b);

protected:
    void writeHeader(QDataStream & dataStream, QFile & file, AssetInformation & info);

    static RawFile::PropertyType typeIndicator(QVariant::Type type);
    static void writeValue(QDataStream & dataStream, const QVariant & value);
    static void writeString(QDataStream & dataStream, const QString & string);

    static const QMap<QVariant::Type, RawFile::PropertyType> s_typeIndicators;

protected:
    QString targetFilePath(const QString & sourcePath, const AssetInformation & info);
    QString suffixesForImage(const AssetInformation & info);
    QString suffixesForCompressedImage(const AssetInformation & info);

protected:
    bool m_headerEnabled;
    bool m_suffixesEnabled;

};

} // namespace glraw
