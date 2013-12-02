
#include <glraw/GLRawFileWriter.h>

#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QVariant>
#include <QFileInfo>

#include <glraw/AssetInformation.h>

namespace glraw
{
    
char GLRawFileWriter::s_magicNumber[4] = { 'c', '6', 'f', '5' };

GLRawFileWriter::GLRawFileWriter()
{
}

GLRawFileWriter::~GLRawFileWriter()
{
}

bool GLRawFileWriter::write(AssetInformation & info, 
    const std::function<void(QDataStream &)> & lambda)
{
    QString target = this->targetFilePath(info, "glraw");
    QFile file(target);

    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Opening file" << target << "failed.";
        return false;
    }

    QDataStream dataStream(&file);
    dataStream.setByteOrder(QDataStream::LittleEndian);

    this->writeHeader(dataStream, file, info);
    lambda(dataStream);

    file.close();
    
    qDebug() << qPrintable(QFileInfo(target).fileName()) << "created.";
    return true;
}

void GLRawFileWriter::writeHeader(QDataStream & dataStream, QFile & file, AssetInformation & info)
{
    dataStream.writeRawData(s_magicNumber, sizeof(s_magicNumber));

    quint64 rawDataOffsetPosition = file.pos();
    dataStream << static_cast<quint64>(0);

    QVariantMap stringProperties = info.stringProperties();
    QVariantMap intProperties = info.intProperties();
    QVariantMap doubleProperties = info.doubleProperties();

    dataStream << static_cast<qint32>(stringProperties.size());
    for (auto it = stringProperties.begin(); it != stringProperties.end(); it++)
    {
        dataStream << it.key().toUtf8();
        dataStream << it.value().toString().toUtf8();
    }

    dataStream << static_cast<qint32>(intProperties.size());
    for (auto it = intProperties.begin(); it != intProperties.end(); it++)
    {
        dataStream << it.key().toUtf8();
        dataStream << static_cast<qint32>(it.value().toInt());
    }

    dataStream << static_cast<qint32>(doubleProperties.size());
    for (auto it = doubleProperties.begin(); it != doubleProperties.end(); it++)
    {
        dataStream << it.key().toUtf8();
        dataStream << it.value().toDouble();
    }

    quint64 rawDataOffset = file.pos();

    file.seek(rawDataOffsetPosition);
    dataStream << rawDataOffset;
    file.seek(rawDataOffset);
}

} // namespace glraw
