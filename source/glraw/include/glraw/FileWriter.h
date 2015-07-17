#pragma once

#include <QtGui/qopengl.h>
#include <QMap>
#include <QString>
#include <QVariant>


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
    virtual ~FileWriter() = default;

    virtual bool write(QByteArray & imageData, AssetInformation & info, const QString& source_path);

    bool headerEnabled() const;
    void setHeaderEnabled(bool b);

    bool suffixesEnabled() const;
    void setSuffixesEnabled(bool b);
    
    bool outputPathSet() const;
    void setOutputPath(const QString & path);

protected:
    void writeHeader(
		QDataStream & dataStream,
		QFile & file,
		AssetInformation & info);

    static RawFile::PropertyType propertyType(QVariant::Type type);

	static void writeValue(QDataStream & dataStream, const QVariant & value);
    static void writeString(QDataStream & dataStream, const QString & string);

protected:
    QString targetFilePath(const QString & sourcePath, const AssetInformation & info);
    QString suffixesForImage(const AssetInformation & info);
    QString suffixesForCompressedImage(const AssetInformation & info);

protected:
    bool m_headerEnabled;
    bool m_suffixesEnabled;
    QString m_outputPath;
};

} // namespace glraw
