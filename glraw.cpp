
#include <cassert>

#include <QMap>
#include <QDebug>
#include <QImage>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QStringList>

#include <QCoreApplication>

enum Format
{
    GL_RGB
,   GL_RGBA
,   GL_DEPTH_COMPONENT
};

enum Type
{
    GL_UNSIGNED_BYTE
};


void man()
{
    qDebug() << "glimg <GLenum-Target-Format> <GLenum-Target-Type> <Input-FilePath>";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(a.arguments().length() != 4)
    {
        qDebug() << "Unexpected number of arguments.";
        man();
        return 0;
    }

    QMap<QString, Format> formats;
    formats["GL_RGB"] = GL_RGB;
    formats["GL_RGBA"] = GL_RGBA;
    formats["GL_DEPTH_COMPONENT"] = GL_DEPTH_COMPONENT;

    const QString farg = a.arguments()[1];
    if(!formats.contains(farg))
    {
        qDebug() << "Unknown target format" << farg << ".";
        man();
        return 0;
    }
    const Format format = formats[farg];


    QMap<QString, Type> types;
    types["GL_UNSIGNED_BYTE"] = GL_UNSIGNED_BYTE;

    const QString targ = a.arguments()[2];
    if(!types.contains(targ))
    {
        qDebug() << "Unknown target type" << targ << ".";
        man();
        return 0;
    }
    const Type type = types[targ];


    const QString iarg = a.arguments()[3];

    if(!QFile::exists(iarg))
    {
        qDebug() << "Input file does not exist.";
        return 0;
    }


    QImage img(iarg);
    if(img.isNull())
    {
        qDebug() << "Loading file from input file failed.";
        return 0;
    }


    // file name

    QMap<Format, QString> formatSuffixes;
    formatSuffixes[GL_RGB]  = "rgb";
    formatSuffixes[GL_RGBA] = "rgba";
    formatSuffixes[GL_DEPTH_COMPONENT] = "d";

    QMap<Type, QString> typeSuffixes;
    typeSuffixes[GL_UNSIGNED_BYTE] = "ub";

    const QString suffixes = QString(".%1.%2.%3.%4")
            .arg(img.width())
            .arg(img.height())
            .arg(formatSuffixes[format])
            .arg(typeSuffixes[type]);

    QFileInfo fi(iarg);

    const QString target = fi.absolutePath() + "/" + fi.baseName() + suffixes;

    // prepare ?


    switch(format)
    {
    case GL_RGB:
        break;
    case GL_RGBA:
        break;
    case GL_DEPTH_COMPONENT:
        break;
    }


    // write

    QFile file(target);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Opening file" << target << "failed.";
        return 0;
    }

    QDataStream output(&file);

    const int w = img.width();
    const int h = img.height();
    const int s = w * h;

    switch(format)
    {
    case GL_RGB:

        assert(img.byteCount() == s * 4);
        for(int i = 0; i < s * 4; i += 4)
        {
            output << img.constBits()[i + 0];
            output << img.constBits()[i + 1];
            output << img.constBits()[i + 2];
        }
        break;

    case GL_RGBA:

        assert(img.byteCount() == s * 4);
        for(int i = 0; i < s * 4; ++i)
        {
            output << img.constBits()[i];
        }
        break;

    case GL_DEPTH_COMPONENT:

        assert(img.byteCount() == s * 4);
        for(int i = 0; i < s * 4; i += 4)
        {
            output << img.constBits()[i + 0];
        }
        break;
    }

    file.close();

    qDebug() << target << "successfully created.";
    return 1;
}
