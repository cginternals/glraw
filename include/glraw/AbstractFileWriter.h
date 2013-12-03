
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
    virtual ~AbstractFileWriter();

    virtual bool write(AssetInformation & info, 
                       const std::function<void(QDataStream &)> & lambda) = 0;

protected:
    static QString targetFilePath(const AssetInformation & info, const QString & fileExtension);

    static const QMap<GLenum, QString> formatSuffixes;
    static const QMap<GLenum, QString> typeSuffixes;

};

} // namespace glraw
