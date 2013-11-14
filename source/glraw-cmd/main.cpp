
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
#include <QCommandLineParser>

#include <glraw/ConvertManager.h>
#include <glraw/RawConverter.h>
#include <glraw/GLRawFileWriter.h>
#include <glraw/MirrorEditor.h>
#include <glraw/ScaleEditor.h>
#include <glraw/GLRawFile.h>
#include <glraw/RawFile.h>

#include "Application.h"
#include "ArgumentsParser.h"

int main(int argc, char * argv[])
{
    Application app(argc, argv);
    
    QCommandLineParser parser;
    parser.setApplicationDescription("Converts Qt supported images to an OpenGL compatible raw format.");
    parser.addHelpOption();
    parser.addVersionOption();
    
    parser.addPositionalArgument("source", "Source file with Qt-supported image format.");
    
    QCommandLineOption mirrorVertical(QStringList() << "mv" << "mirror-vertical", "Mirrors the image vertically.");
    QCommandLineOption mirrorHorizontal(QStringList() << "mh" << "mirror-horizontal", "Mirrors the image horizontally.");
    
    QCommandLineOption scale(QStringList() << "s" << "scale", "Scales the image.", "decimal");
    QCommandLineOption widthScale(QStringList() << "ws" << "width-scale", "Scales the width.", "decimal");
    QCommandLineOption heightScale(QStringList() << "hs" << "height-scale", "Scales the height.", "decimal");

    QCommandLineOption width("width", "Sets the width.", "integer");
    QCommandLineOption height("height", "Sets the height.", "integer");
    
    QCommandLineOption transformationMode(
        "transform-mode",
        "Transformation mode used for resizing        "
        "(default: nearest)",
        "mode"
    );
    
    QCommandLineOption aspectRatioMode(
        "aspect-ratio-mode",
        "Aspect ratio mode used for resizing          "
        "(default: IgnoreAspectRatio)",
        "mode"
    );
    
    QCommandLineOption format(
        QStringList() << "f" << "format",
        "Output format (default: GL_RGBA)",
        "format"
    );
    
    QCommandLineOption type(
        QStringList() << "t" << "type",
        "Output type (default: GL_INT)",
        "type"
    );
    
    parser.addOption(format);
    parser.addOption(type);
    parser.addOption(mirrorVertical);
    parser.addOption(mirrorHorizontal);
    parser.addOption(scale);
    parser.addOption(widthScale);
    parser.addOption(heightScale);
    parser.addOption(width);
    parser.addOption(height);
    parser.addOption(transformationMode);
    parser.addOption(aspectRatioMode);
    
    if (app.arguments().size() == 1)
        parser.showHelp();
    
    parser.process(app);
    
    QString source = parser.positionalArguments().at(0);
    
    glraw::RawConverter converter;
    glraw::GLRawFileWriter writer;
    glraw::ConvertManager manager(converter, writer);
    
    manager.process(source);
    
    return 0;
}

// TODO Why?
// if (GL_BGR != parser.format() && GL_BGRA != parser.format())
//     image = image.rgbSwapped();



