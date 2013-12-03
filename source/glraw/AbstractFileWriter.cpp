
#include <glraw/AbstractFileWriter.h>

#include <QDebug>
#include <QFileInfo>

#include <glraw/Enumerations.h>
#include <glraw/AssetInformation.h>

namespace glraw
{

const QMap<Format, QString> AbstractFileWriter::formatSuffixes = {
    { RAW_GL_RED, "r" },
    { RAW_GL_RG, "rg" },
    { RAW_GL_RGB, "rgb" },
    { RAW_GL_BGR, "bgr" },
    { RAW_GL_RGBA, "rgba" },
    { RAW_GL_BGRA, "bgra" }
};
const QMap<Type, QString> AbstractFileWriter::typeSuffixes = {
    { RAW_GL_UNSIGNED_BYTE, "ub" },
    { RAW_GL_BYTE, "b" },
    { RAW_GL_UNSIGNED_SHORT, "us" },
    { RAW_GL_SHORT, "s" },
    { RAW_GL_UNSIGNED_INT, "ui" },
    { RAW_GL_INT, "i" },
    { RAW_GL_FLOAT, "f" }
};

AbstractFileWriter::~AbstractFileWriter()
{
}

QString AbstractFileWriter::targetFilePath(const AssetInformation & info, const QString & fileExtension)
{
    const Format format = static_cast<Format>(info.property("format").toInt());
    const Type type = static_cast<Type>(info.property("type").toInt());
    
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
