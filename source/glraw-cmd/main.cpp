
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

#include <glraw/ConvertManager.h>
#include <glraw/RawConverter.h>
#include <glraw/RawFileWriter.h>

#include "ArgumentsParser.h"

int main(int argc, char * argv[])
{
    QCoreApplication a(argc, argv);
    
    ArgumentsParser parser(a.arguments());
    
    if (!parser.isValid())
        return 0;
    
    glraw::RawConverter converter;
    glraw::RawFileWriter writer;
    glraw::ConvertManager manager(converter, writer);
    
    if (!manager.process(parser.filePath()))
        return 0;
    
    return 0;
}

//int old_main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);
//
//    ArgumentsParser parser(a.arguments());
//
//    if (!parser.isValid())
//        return 0;
//
//    if(!QFile::exists(parser.filePath()))
//    {
//        qDebug() << "Input file does not exist.";
//        return 0;
//    }
//
//    QImage image(parser.filePath());
//    if(image.isNull())
//    {
//        qDebug() << "Loading file from input file failed.";
//        return 0;
//    }
//
//    // scaling
//
//    if (parser.hasValidScale())
//    {
//        QSize size = image.size() * parser.scale();
//        image = image.scaled(size, parser.aspectRatioMode(), parser.transformationMode());
//    }
//
//    if (parser.hasValidWidth())
//    {
//        QSize size = image.size();
//        size.setWidth(parser.width());
//        image = image.scaled(size, parser.aspectRatioMode(), parser.transformationMode());
//    }
//    else if (parser.hasValidWidthScale())
//    {
//        QSize size = image.size();
//        size.setWidth(size.width() * parser.widthScale());
//        image = image.scaled(size, parser.aspectRatioMode(), parser.transformationMode());
//    }
//
//    if (parser.hasValidHeight())
//    {
//        QSize size = image.size();
//        size.setHeight(parser.height());
//        image = image.scaled(size, parser.aspectRatioMode(), parser.transformationMode());
//    }
//    else if (parser.hasValidHeightScale())
//    {
//        QSize size = image.size();
//        size.setHeight(size.height() * parser.heightScale());
//        image = image.scaled(size, parser.aspectRatioMode(), parser.transformationMode());
//    }
//
//    // file name
//
//    QMap<Format, QString> formatSuffixes;
//    formatSuffixes[GL_RED]  = "r";
//    formatSuffixes[GL_RG]   = "rg";
//    formatSuffixes[GL_RGB]  = "rgb";
//    formatSuffixes[GL_BGR]  = "bgr";
//    formatSuffixes[GL_RGBA] = "rgba";
//    formatSuffixes[GL_BGRA] = "bgra";
//
//    QMap<Type, QString> typeSuffixes;
//    typeSuffixes[GL_UNSIGNED_BYTE]  = "ub";
//    typeSuffixes[GL_BYTE]           = "b";
//    typeSuffixes[GL_UNSIGNED_SHORT] = "us";
//    typeSuffixes[GL_SHORT]          = "s";
//    typeSuffixes[GL_UNSIGNED_INT]   = "ui";
//    typeSuffixes[GL_INT]            = "i";
//    typeSuffixes[GL_FLOAT]          = "f";
//
//    const QString suffixes = QString(".%1.%2.%3.%4.raw")
//            .arg(image.width())
//            .arg(image.height())
//            .arg(formatSuffixes[parser.format()])
//            .arg(typeSuffixes[parser.type()]);
//
//    QFileInfo fi(parser.filePath());
//
//    const QString target = fi.absolutePath() + "/" + fi.baseName() + suffixes;
//
//    // prepare ?
//
//    if (GL_BGR != parser.format() && GL_BGRA != parser.format())
//        image = image.rgbSwapped();
//
//    image = image.mirrored(parser.mirrorHorizontal(), !parser.mirrorVertical());
//
//    // write
//
//    QFile file(target);
//    if(!file.open(QIODevice::WriteOnly))
//    {
//        qDebug() << "Opening file" << target << "failed.";
//        return 0;
//    }
//
//    QDataStream output(&file);
//
//    switch(parser.type())
//    {
//    case GL_UNSIGNED_BYTE:
//        write<unsigned char>(output, image, parser.format());
//        break;
//    case GL_BYTE:
//        write<char>(output, image, parser.format());
//        break;
//    case GL_UNSIGNED_SHORT:
//        write<unsigned short>(output, image, parser.format());
//        break;
//    case GL_SHORT:
//        write<short>(output, image, parser.format());
//        break;
//    case GL_UNSIGNED_INT:
//        write<unsigned int>(output, image, parser.format());
//        break;
//    case GL_INT:
//        write<int>(output, image, parser.format());
//        break;
//    case GL_FLOAT:
//        write<float>(output, image, parser.format());
//        break;
//    default:
//        qWarning() << "Should not happen :P.";
//        break;
//    }
//
//    file.close();
//
//    qDebug() << qPrintable(target) << "created.";
//    return 1;
//}

