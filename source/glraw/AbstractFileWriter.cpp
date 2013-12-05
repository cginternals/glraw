
#include <glraw/AbstractFileWriter.h>

#include <QtGui/qopengl.h>
#include <QDebug>
#include <QFileInfo>
#include <QByteArray>
#include <QString>

#include <glraw/AssetInformation.h>

namespace glraw
{

const QMap<GLenum, QString> AbstractFileWriter::s_formatSuffixes = {
    { GL_RED, "r" },
    { GL_RED, "g" },
    { GL_RED, "b" },
    { GL_RG, "rg" },
    { GL_RGB, "rgb" },
    { GL_BGR, "bgr" },
    { GL_RGBA, "rgba" },
    { GL_BGRA, "bgra" }
};
const QMap<GLenum, QString> AbstractFileWriter::s_typeSuffixes = {
    { GL_UNSIGNED_BYTE, "ub" },
    { GL_BYTE, "b" },
    { GL_UNSIGNED_SHORT, "us" },
    { GL_SHORT, "s" },
    { GL_UNSIGNED_INT, "ui" },
    { GL_INT, "i" },
    { GL_FLOAT, "f" }
};

AbstractFileWriter::AbstractFileWriter()
:   m_suffixesEnabled(true)
{
}
    
AbstractFileWriter::~AbstractFileWriter()
{
}
    
bool AbstractFileWriter::suffixesEnabled() const
{
    return m_suffixesEnabled;
}
    
void AbstractFileWriter::setSuffixesEnabled(bool b)
{
    m_suffixesEnabled = b;
}

QString AbstractFileWriter::targetFilePath(const AssetInformation & info, const QString & fileExtension)
{
    QFileInfo fileInfo(info.property("inputFilePath").toString());
    
    if (!m_suffixesEnabled)
        return fileInfo.absolutePath() + "/" + fileInfo.baseName() + "." + fileExtension;
    
    GLenum format = static_cast<GLenum>(info.property("format").toInt());
    GLenum type = static_cast<GLenum>(info.property("type").toInt());
       
    const QString suffixes = QString(".%1.%2.%3.%4.%5")
        .arg(info.property("width").toInt())
        .arg(info.property("height").toInt())
        .arg(s_formatSuffixes[format])
        .arg(s_typeSuffixes[type])
        .arg(fileExtension);
    
    return fileInfo.absolutePath() + "/" + fileInfo.baseName() + suffixes;
}

} // namespace glraw
