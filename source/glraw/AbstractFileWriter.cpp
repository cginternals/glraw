
#include <glraw/AbstractFileWriter.h>

#include <QDebug>
#include <QFileInfo>

#include <glraw/Enumerations.h>
#include <glraw/AssetInformation.h>

namespace glraw
{

AbstractFileWriter::~AbstractFileWriter()
{
}

QString AbstractFileWriter::targetFilePath(const AssetInformation & info, const QString & fileExtension)
{
    const Format format = static_cast<Format>(info.property("format").toInt());
    const Type type = static_cast<Type>(info.property("type").toInt());
    
    QMap<Format, QString> formatSuffixes;
    formatSuffixes[GL_RED]  = "r";
    formatSuffixes[GL_RG]   = "rg";
    formatSuffixes[GL_RGB]  = "rgb";
    formatSuffixes[GL_BGR]  = "bgr";
    formatSuffixes[GL_RGBA] = "rgba";
    formatSuffixes[GL_BGRA] = "bgra";
    
    QMap<Type, QString> typeSuffixes;
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
