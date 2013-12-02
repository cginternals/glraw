
#include <glraw/AbstractFileWriter.h>

#include <QtGui/qopengl.h>
#include <QDebug>
#include <QFileInfo>

#include <glraw/AssetInformation.h>

namespace glraw
{

AbstractFileWriter::~AbstractFileWriter()
{
}

QString AbstractFileWriter::targetFilePath(const AssetInformation & info, const QString & fileExtension)
{
    GLenum format = static_cast<GLenum>(info.property("format").toInt());
    GLenum type = static_cast<GLenum>(info.property("type").toInt());
    
    QMap<GLenum, QString> formatSuffixes;
    formatSuffixes[GL_RED]   = "r";
    formatSuffixes[GL_GREEN] = "g";
    formatSuffixes[GL_BLUE]  = "b";
    formatSuffixes[GL_RG]    = "rg";
    formatSuffixes[GL_RGB]   = "rgb";
    formatSuffixes[GL_BGR]   = "bgr";
    formatSuffixes[GL_RGBA]  = "rgba";
    formatSuffixes[GL_BGRA]  = "bgra";
    
    QMap<GLenum, QString> typeSuffixes;
    typeSuffixes[GL_UNSIGNED_BYTE]  = "ub";
    typeSuffixes[GL_BYTE]           = "b";
    typeSuffixes[GL_UNSIGNED_SHORT] = "us";
    typeSuffixes[GL_SHORT]          = "s";
    typeSuffixes[GL_UNSIGNED_INT]   = "ui";
    typeSuffixes[GL_INT]            = "i";
    typeSuffixes[GL_FLOAT]          = "f";
    
    const QString suffixes = QString(".%1.%2.%3.%4.%5")
        .arg(info.property("width").toInt())
        .arg(info.property("height").toInt())
        .arg(formatSuffixes[format])
        .arg(typeSuffixes[type])
        .arg(fileExtension);
    
    QFileInfo fileInfo(info.property("inputFilePath").toString());
    
    return fileInfo.absolutePath() + "/" + fileInfo.baseName() + suffixes;
}

} // namespace glraw
