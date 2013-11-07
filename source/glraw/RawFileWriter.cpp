
#include <glraw/RawFileWriter.h>

#include <QDebug>
#include <QDataStream>
#include <QFile>

namespace glraw
{

RawFileWriter::RawFileWriter()
{
}

RawFileWriter::~RawFileWriter()
{
}

bool RawFileWriter::write(AssetInformation & info, 
    const std::function<void(QDataStream &)> & lambda)
{
    QString target = this->targetFilePath(info, "raw");
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
