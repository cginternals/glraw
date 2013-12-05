
#pragma once

#include <glraw/glraw.h>

#include <glraw/WriterInterface.h>

#include <QMap>
#include <QString>
#include <QtGui/qopengl.h>

namespace glraw
{

class GLRAW_API AbstractFileWriter : public WriterInterface
{
public:
    AbstractFileWriter();
    virtual ~AbstractFileWriter();

    virtual bool write(AssetInformation & info, 
                       const std::function<void(QDataStream &)> & lambda) = 0;
    
    bool suffixesEnabled() const;
    void setSuffixesEnabled(bool b);

protected:
    QString targetFilePath(const AssetInformation & info, const QString & fileExtension);

    static const QMap<GLenum, QString> s_formatSuffixes;
    static const QMap<GLenum, QString> s_typeSuffixes;

    bool m_suffixesEnabled;

};

} // namespace glraw
