
#include <glraw/FileWriter.h>

#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>

#include <glraw/AssetInformation.h>
#include <glraw/FileNameSuffix.h>


namespace glraw
{
    
FileWriter::FileWriter(bool headerEnabled, bool suffixesEnabled)
:   m_headerEnabled(headerEnabled)
,   m_suffixesEnabled(suffixesEnabled)
{
}

bool FileWriter::write(QByteArray & imageData, AssetInformation & info, const QString & source_path)
{
    QString target = targetFilePath(source_path, info);
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

    dataStream.writeRawData(imageData.data(), imageData.size());

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

bool FileWriter::outputPathSet() const
{
    return !m_outputPath.isEmpty();
}

void FileWriter::setOutputPath(const QString & path)
{
    m_outputPath = path;
}

void FileWriter::writeHeader(QDataStream & dataStream, QFile & file, AssetInformation & info)
{
    if (info.properties().empty())
        return;

    dataStream << static_cast<quint16>(RawFile::s_signature);

    quint64 rawDataOffsetPosition = file.pos();
    dataStream << static_cast<quint64>(0);

    QMapIterator<QVariantMap::key_type, QVariantMap::mapped_type> iterator(info.properties());

    while (iterator.hasNext())
    {
        iterator.next();

        QString key = iterator.key();
        QVariant value = iterator.value();

		RawFile::PropertyType type = propertyType(value.type());

        if (type == RawFile::PropertyType::Unknown)
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

RawFile::PropertyType FileWriter::propertyType(QVariant::Type type)
{
	static const QMap<QVariant::Type, RawFile::PropertyType> propertyTypesByVariantType =
	{
		{ QVariant::Int,    RawFile::PropertyType::Int    },
		{ QVariant::Double, RawFile::PropertyType::Double },
		{ QVariant::String, RawFile::PropertyType::String }
	};

	return propertyTypesByVariantType.value(type, RawFile::PropertyType::Unknown);
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
    
    const QString path = outputPathSet() ? m_outputPath : fileInfo.absolutePath();
    
    if (!m_suffixesEnabled)
        return path + "/" + fileInfo.baseName() + "." + fileExtension;
    
    QString suffixes;
    if (info.propertyExists("compressedFormat"))
        suffixes = suffixesForCompressedImage(info);
    else
        suffixes = suffixesForImage(info);
    
    return path + "/" + fileInfo.baseName() + suffixes + "." + fileExtension;
}

QString FileWriter::suffixesForImage(const AssetInformation & info)
{
	const FileNameSuffix suffix(info.property("width").toInt(), info.property("height").toInt(),
		static_cast<GLenum>(info.property("format").toInt()), static_cast<GLenum>(info.property("type").toInt()));

	return suffix.get();
}

QString FileWriter::suffixesForCompressedImage(const AssetInformation & info)
{
	const FileNameSuffix suffix(info.property("width").toInt(), info.property("height").toInt(),
		static_cast<GLenum>(info.property("compressedFormat").toInt()));

	return suffix.get();
}

} // namespace glraw
