
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

bool RawFileWriter::write(const QByteArray & imageData, AssetInformation & info)
{
    QString target = this->targetFilePath(info, "raw");
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
