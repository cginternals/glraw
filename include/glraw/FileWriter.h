#pragma once

#include <cstdint>

#include <glraw/glraw.h>

#include <glraw/RawFile.h>

#include <QtGui/qopengl.h>
#include <QMap>
#include <QVariant>

class QFile;
class QDataStream;

namespace glraw
{

class AssetInformation;

class GLRAW_API FileWriter
{
public:
    FileWriter(bool headerEnabled = true, bool suffixesEnabled = true);
    ~FileWriter();

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

    static const QMap<GLenum, QString> s_formatSuffixes;
    static const QMap<GLenum, QString> s_typeSuffixes;

protected:
    bool m_headerEnabled;
    bool m_suffixesEnabled;

};

} // namespace glraw
