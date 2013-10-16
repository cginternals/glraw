
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

#include <glraw-version.h>


enum Format
{
    GL_RED
,   GL_RG
,   GL_RGB
,   GL_BGR
,   GL_RGBA
,   GL_BGRA
,   GL_STENCIL_INDEX
,   GL_DEPTH_COMPONENT
,   GL_DEPTH_STENCIL
};

enum Type
{
    GL_UNSIGNED_BYTE
,   GL_BYTE
,   GL_UNSIGNED_SHORT
,   GL_SHORT
,   GL_UNSIGNED_INT
,   GL_INT
,   GL_FLOAT
};


QMap<QString, Format> g_formats;
QMap<QString, Type> g_types;
QVector<QString> g_options;
QStringList g_manuals;
QStringList g_versions;

void initialize()
{
    g_formats["GL_RED"] = GL_RED;
    g_formats["GL_RG"] = GL_RG;
    g_formats["GL_RGB"] = GL_RGB;
    g_formats["GL_BGR"] = GL_BGR;
    g_formats["GL_RGBA"] = GL_RGBA;
    g_formats["GL_BGRA"] = GL_BGRA;
    //g_formats["GL_STENCIL_INDEX"] = GL_STENCIL_INDEX;
    //g_formats["GL_DEPTH_COMPONENT"] = GL_DEPTH_COMPONENT;
    //g_formats["GL_DEPTH_STENCIL"] = GL_DEPTH_STENCIL;

    g_types["GL_UNSIGNED_BYTE"] = GL_UNSIGNED_BYTE;
    g_types["GL_BYTE"] = GL_BYTE;
    g_types["GL_UNSIGNED_SHORT"] = GL_UNSIGNED_SHORT;
    g_types["GL_SHORT"] = GL_SHORT;
    g_types["GL_UNSIGNED_INT"] = GL_UNSIGNED_INT;
    g_types["GL_INT"] = GL_INT;
    g_types["GL_FLOAT"] = GL_FLOAT;

    g_options << "--mirror-horizontal"  << "-mh" ;
    g_options << "--mirror-vertical"    << "-mv" ;
    g_options << "--as-bgr"             << "-bgr";

    g_manuals  << "--help" << "-h" << "/?";
    g_versions << "--version" << "-v";
}

static const int option_mh  = 0;
static const int option_mv  = 2;
static const int option_bgr = 4;

void man()
{
    qDebug() << "";
    qDebug() << GLRAW_PROJECT_NAME << GLRAW_VERSION << "by" << GLRAW_AUTHOR_ORGANIZATION;
    qDebug() << "Converts an input image to an OpenGL compatible format.";
    qDebug() << "";
    qDebug() << "usage: glraw <Target-Format> <Target-Type> [<Options>] <Input-FilePath>";
    qDebug() << "";
    qDebug() << "<Target-Formats>:";
    qDebug().nospace() << "  " << qPrintable(QStringList(g_formats.keys()).join("\n  "));
    qDebug() << "";
    qDebug() << "<Target-Types>:";
    qDebug().nospace() << "  " << qPrintable(QStringList(g_types.keys()).join("\n  "));
    qDebug() << "";
    qDebug() << "<Options>:";
    qDebug().nospace() << "  " << qPrintable(g_options[option_mh])  << " (" << qPrintable(g_options[option_mh + 1])  << ")";
    qDebug().nospace() << "  " << qPrintable(g_options[option_mv])  << " (" << qPrintable(g_options[option_mv + 1])  << ")";
    qDebug().nospace() << "  " << qPrintable(g_options[option_bgr]) << " (" << qPrintable(g_options[option_bgr + 1]) << ")";
    qDebug() << "";
}

void version()
{
    qDebug() << GLRAW_PROJECT_NAME << GLRAW_VERSION;
    qDebug() << "";
}

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

    initialize();

    if (a.arguments().size() == 1 || g_manuals.contains(a.arguments()[1]))
    {
        man();
        return 0;
    }

    if (g_versions.contains(a.arguments()[1]))
    {
        version();
        return 0;
    }

    const QString farg = a.arguments()[1];
    if(!g_formats.contains(farg))
    {
        qWarning() << "Unknown Target-Format" << qPrintable(farg) << ".";
        return 0;
    }
    const Format format = g_formats[farg];


    const QString targ = a.arguments()[2];
    if(!g_types.contains(targ))
    {
        qWarning() << "Unknown Target-Type" << qPrintable(targ) << ".";
        return 0;
    }
    const Type type = g_types[targ];

    const QStringList args(a.arguments());

    const bool mh  = args.contains(g_options[option_mh])  || args.contains(g_options[option_mh + 1]);
    const bool mv  = args.contains(g_options[option_mv])  || args.contains(g_options[option_mv + 1]);
    const bool bgr = args.contains(g_options[option_bgr]) || args.contains(g_options[option_bgr + 1]);

    // catch unknown args

    QStringList unknowns;
    for (int i = 1; i < args.size() - 1; ++i)
    {
        if (!g_options.contains(args[i]) && !g_formats.contains(args[i]) && !g_types.contains(args[i]))
            unknowns << args[i];
    }
    if (!unknowns.isEmpty())
        qWarning() << "The following unknown arguments will be ignored:" << unknowns.join(", ");

    const QString iarg = a.arguments().last();

    if(!QFile::exists(iarg))
    {
        qDebug() << "Input file does not exist.";
        return 0;
    }


    QImage image(iarg);
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
            .arg(formatSuffixes[format])
            .arg(typeSuffixes[type]);

    QFileInfo fi(iarg);

    const QString target = fi.absolutePath() + "/" + fi.baseName() + suffixes;

    // prepare ?

    if (GL_BGR != format && GL_BGRA != format)
        image = image.rgbSwapped();

    image = image.mirrored(mh, !mv);


    // write

    QFile file(target);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Opening file" << target << "failed.";
        return 0;
    }

    QDataStream output(&file);

    switch(type)
    {
    case GL_UNSIGNED_BYTE:
        write<unsigned char>(output, image, format);
        break;
    case GL_BYTE:
        write<char>(output, image, format);
        break;
    case GL_UNSIGNED_SHORT:
        write<unsigned short>(output, image, format);
        break;
    case GL_SHORT:
        write<short>(output, image, format);
        break;
    case GL_UNSIGNED_INT:
        write<unsigned int>(output, image, format);
        break;
    case GL_INT:
        write<int>(output, image, format);
        break;
    case GL_FLOAT:
        write<float>(output, image, format);
        break;
    default:
        qWarning() << "Should not happen :P.";
        break;
    }

    file.close();

    qDebug() << qPrintable(target) << "created.";
    return 1;
}

