
#include <glraw/FileWriter.h>

#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>

#include <glraw/AssetInformation.h>

namespace glraw
{
    
const QMap<QVariant::Type, RawFile::PropertyType> FileWriter::s_typeIndicators = {
    { QVariant::Int, RawFile::IntType },
    { QVariant::Double, RawFile::DoubleType },
    { QVariant::String, RawFile::StringType }
};

const QMap<GLenum, QString> FileWriter::s_formatSuffixes = {
    { GL_RED, "r" },
    { GL_RED, "g" },
    { GL_RED, "b" },
    { GL_RG, "rg" },
    { GL_RGB, "rgb" },
    { GL_BGR, "bgr" },
    { GL_RGBA, "rgba" },
    { GL_BGRA, "bgra" }
};

const QMap<GLenum, QString> FileWriter::s_typeSuffixes = {
    { GL_UNSIGNED_BYTE, "ub" },
    { GL_BYTE, "b" },
    { GL_UNSIGNED_SHORT, "us" },
    { GL_SHORT, "s" },
    { GL_UNSIGNED_INT, "ui" },
    { GL_INT, "i" },
    { GL_FLOAT, "f" }
};

FileWriter::FileWriter(bool headerEnabled, bool suffixesEnabled)
:   m_headerEnabled(headerEnabled)
,   m_suffixesEnabled(suffixesEnabled)
{
}

FileWriter::~FileWriter()
{
}

bool FileWriter::write(const QByteArray & imageData,
    const QString & sourcePath, AssetInformation & info)
{
    QString target = targetFilePath(sourcePath, info);
    QFile file(target);

    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Opening file" << target << "failed.";
        return false;
    }

    QDataStream dataStream(&file);

    if (m_headerEnabled)
    {
        dataStream.setByteOrder(QDataStream::LittleEndian);
        writeHeader(dataStream, file, info);
    }

    dataStream.writeBytes(imageData.data(), imageData.length());

    file.close();
    
    qDebug() << qPrintable(QFileInfo(target).fileName()) << "created.";
    return true;
}

bool FileWriter::headerEnabled() const
{
    return m_headerEnabled;
}

void FileWriter::setHeaderEnabled(bool b)
{
    m_headerEnabled = b;
}

bool FileWriter::suffixesEnabled() const
{
    return m_suffixesEnabled;
}

void FileWriter::setSuffixesEnabled(bool b)
{
    m_suffixesEnabled = b;
}

void FileWriter::writeHeader(QDataStream & dataStream, QFile & file, AssetInformation & info)
{
    if (info.properties().empty())
        return;

    dataStream << static_cast<quint16>(RawFile::s_magicNumber);

    quint64 rawDataOffsetPosition = file.pos();
    dataStream << static_cast<quint64>(0);

    QMapIterator<QVariantMap::key_type, QVariantMap::mapped_type> iterator(info.properties());

    while (iterator.hasNext())
    {
        iterator.next();

        QString key = iterator.key();
        QVariant value = iterator.value();

        int type = typeIndicator(value.type());

        if (type == RawFile::Unknown)
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

RawFile::PropertyType FileWriter::typeIndicator(QVariant::Type type)
{
    return s_typeIndicators.value(type, RawFile::Unknown);
}

void FileWriter::writeValue(QDataStream & dataStream, const QVariant & value)
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

void FileWriter::writeString(QDataStream & dataStream, const QString & string)
{
    QByteArray bytes = string.toUtf8();
    dataStream.writeRawData(bytes.data(), bytes.length());
    dataStream << static_cast<qint8>(0);
}

QString FileWriter::targetFilePath(const QString & sourcePath, const AssetInformation & info)
{
    QFileInfo fileInfo(sourcePath);

    const QString fileExtension = m_headerEnabled ? "glraw" : "raw";
    
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
