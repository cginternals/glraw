
#pragma once

#include <glraw/glraw.h>

#include <QMap>
#include <QtGui/qopengl.h>

class QByteArray;
class QString;

namespace glraw
{
    
class AssetInformation;

class GLRAW_API AbstractFileWriter
{
public:
    AbstractFileWriter();
    virtual ~AbstractFileWriter();

    virtual bool write(const QByteArray & imageData,
                       AssetInformation & info) = 0;
    
    bool suffixesEnabled() const;
    void setSuffixesEnabled(bool b);

protected:
    QString targetFilePath(const AssetInformation & info, const QString & fileExtension);

    static const QMap<GLenum, QString> s_formatSuffixes;
    static const QMap<GLenum, QString> s_typeSuffixes;

    bool m_suffixesEnabled;

};

} // namespace glraw
