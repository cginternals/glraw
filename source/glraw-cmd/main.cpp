
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



