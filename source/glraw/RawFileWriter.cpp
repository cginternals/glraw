
#include <glraw/RawFileWriter.h>

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDataStream>
#include <glraw/Enumerations.h>
#include <glraw/AssetInformation.h>

namespace glraw
{

RawFileWriter::RawFileWriter()
{
}

RawFileWriter::~RawFileWriter()
{
}

QString RawFileWriter::targetFilePath(const AssetInformation & info)
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
    
    const QString suffixes = QString(".%1.%2.%3.%4.raw")
        .arg(info.property("width").toInt())
        .arg(info.property("height").toInt())
        .arg(formatSuffixes[format])
        .arg(typeSuffixes[type]);
    
    QFileInfo fileInfo(info.property("inputFilePath").toString());
    
    return fileInfo.absolutePath() + "/" + fileInfo.baseName() + suffixes;
}

bool RawFileWriter::write(AssetInformation & info, 
    const std::function<void(QDataStream &)> & lambda)
{
    QString target = this->targetFilePath(info);
    QFile file(target);

    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Opening file" << target << "failed.";
        return false;
    }

    QDataStream dataStream(&file);
    lambda(dataStream);

    file.close();

    qDebug() << qPrintable(target) << "created.";
    return true;
}

} // namespace glraw
