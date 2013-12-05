
#include <glraw/RawFileWriter.h>

#include <QDebug>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>

namespace glraw
{

RawFileWriter::RawFileWriter()
{
}

RawFileWriter::~RawFileWriter()
{
}

bool RawFileWriter::write(const QByteArray & imageData, const QString & sourcePath, AssetInformation & info)
{
    QString target = targetFilePath(sourcePath, "raw", info);
    QFile file(target);

    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Opening file" << target << "failed.";
        return false;
    }

    QDataStream dataStream(&file);
    dataStream.writeBytes(imageData.data(), imageData.length());

    file.close();

    qDebug() << qPrintable(QFileInfo(target).fileName()) << "created.";
    return true;
}

} // namespace glraw
