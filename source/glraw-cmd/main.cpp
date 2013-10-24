
#include <cassert>
#include <type_traits>

#include <QMap>
#include <QDebug>
#include <QImage>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QStringList>

#include <QCoreApplication>

#include "ArgumentsParser.h"

template<typename T>
void write(
    QDataStream & output
,   const QImage & image
,   const Format format)
{
    const bool r = (GL_RED == format || GL_RG == format || GL_RGB == format || GL_RGBA == format || GL_BGR == format || GL_BGRA == format);
    const bool g = (GL_RG == format || GL_RGB == format || GL_RGBA == format || GL_BGR == format || GL_BGRA == format);
    const bool b = (GL_RGB == format || GL_RGBA == format || GL_BGR == format || GL_BGRA == format);
    const bool a = (GL_RGBA == format || GL_BGRA == format);

    const bool grayscale = (GL_DEPTH_COMPONENT == format || GL_DEPTH_STENCIL == format || GL_STENCIL_INDEX == format);

    const int w = image.width();
    const int h = image.height();
    const int s = w * h;

    const float scale = std::is_same<T, float>::value ? 1.f / 255.f : 1.f;

    assert(image.byteCount() == s * 4);
    for(int i = 0; i < s * 4; i += 4)
    {
        if (r)
            output << static_cast<T>(scale * image.constBits()[i + 0]);
        if (g)
            output << static_cast<T>(scale * image.constBits()[i + 1]);
        if (b)
            output << static_cast<T>(scale * image.constBits()[i + 2]);
        if (a)
            output << static_cast<T>(scale * image.constBits()[i + 3]);
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ArgumentsParser parser(a.arguments());

    if (!parser.isValid())
        return 0;

    if(!QFile::exists(parser.filePath()))
    {
        qDebug() << "Input file does not exist.";
        return 0;
    }

    QImage image(parser.filePath());
    if(image.isNull())
    {
        qDebug() << "Loading file from input file failed.";
        return 0;
    }

    // file name

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
            .arg(image.width())
            .arg(image.height())
            .arg(formatSuffixes[parser.format()])
            .arg(typeSuffixes[parser.type()]);

    QFileInfo fi(parser.filePath());

    const QString target = fi.absolutePath() + "/" + fi.baseName() + suffixes;

    // prepare ?

    if (GL_BGR != parser.format() && GL_BGRA != parser.format())
        image = image.rgbSwapped();

    image = image.mirrored(parser.mirrorHorizontal(), !parser.mirrorVertical());


    // write

    QFile file(target);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Opening file" << target << "failed.";
        return 0;
    }

    QDataStream output(&file);

    switch(parser.type())
    {
    case GL_UNSIGNED_BYTE:
        write<unsigned char>(output, image, parser.format());
        break;
    case GL_BYTE:
        write<char>(output, image, parser.format());
        break;
    case GL_UNSIGNED_SHORT:
        write<unsigned short>(output, image, parser.format());
        break;
    case GL_SHORT:
        write<short>(output, image, parser.format());
        break;
    case GL_UNSIGNED_INT:
        write<unsigned int>(output, image, parser.format());
        break;
    case GL_INT:
        write<int>(output, image, parser.format());
        break;
    case GL_FLOAT:
        write<float>(output, image, parser.format());
        break;
    default:
        qWarning() << "Should not happen :P.";
        break;
    }

    file.close();

    qDebug() << qPrintable(target) << "created.";
    return 1;
}

