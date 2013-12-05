
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
    
QMap<QVariant::Type, GLRawFile::PropertyType> GLRawFileWriter::s_typeIndicators = {
    { QVariant::Int, GLRawFile::IntType },
    { QVariant::Double, GLRawFile::DoubleType },
    { QVariant::String, GLRawFile::StringType }
};

GLRawFileWriter::GLRawFileWriter()
{
}

GLRawFileWriter::~GLRawFileWriter()
{
}

bool GLRawFileWriter::write(const QByteArray & imageData, AssetInformation & info)
{
    QString target = targetFilePath(info, "glraw");
    QFile file(target);

    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Opening file" << target << "failed.";
        return false;
    }

    QDataStream dataStream(&file);
    dataStream.setByteOrder(QDataStream::LittleEndian);

    writeHeader(dataStream, file, info);
    
    dataStream.writeBytes(imageData.data(), imageData.length());

    file.close();
    
    qDebug() << qPrintable(QFileInfo(target).fileName()) << "created.";
    return true;
}

void GLRawFileWriter::writeHeader(QDataStream & dataStream, QFile & file, AssetInformation & info)
{
    if (info.properties().empty())
        return;

    dataStream << static_cast<quint16>(GLRawFile::s_magicNumber);

    quint64 rawDataOffsetPosition = file.pos();
    dataStream << static_cast<quint64>(0);

    QMapIterator<QVariantMap::key_type, QVariantMap::mapped_type> iterator(info.properties());

    while (iterator.hasNext())
    {
        iterator.next();

        QString key = iterator.key();
        QVariant value = iterator.value();

        int type = typeIndicator(value.type());

        if (type == GLRawFile::Unknown)
            continue;

        dataStream << static_cast<uint8_t>(type);
        writeString(dataStream, key);

        writeValue(dataStream, value);
    }

    quint64 rawDataOffset = file.pos();

    file.seek(rawDataOffsetPosition);
    dataStream << rawDataOffset;
    file.seek(rawDataOffset);
}

GLRawFile::PropertyType GLRawFileWriter::typeIndicator(QVariant::Type type)
{
    return s_typeIndicators.value(type, GLRawFile::Unknown);
}

void GLRawFileWriter::writeValue(QDataStream & dataStream, const QVariant & value)
{
    switch (value.type())
    {
        case QVariant::Int:
            dataStream << static_cast<qint32>(value.toInt());
            break;
        case QVariant::Double:
            dataStream << value.toDouble();
            break;
        case QVariant::String:
            writeString(dataStream, value.toString());
            break;
        default:
            dataStream << static_cast<qint8>(0);
    }
}

void GLRawFileWriter::writeString(QDataStream & dataStream, const QString & string)
{
    QByteArray bytes = string.toUtf8();
    dataStream.writeRawData(bytes.data(), bytes.length());
    dataStream << static_cast<qint8>(0);
}

} // namespace glraw
