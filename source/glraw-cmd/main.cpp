
#include <cassert>
#include <type_traits>
#include <iostream>

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
#include "Builder.h"

void messageHandler(QtMsgType type, const QMessageLogContext & context, const QString & message)
{
    if (type == QtDebugMsg)
        return;

    std::cout << message.toStdString() << std::endl;
}

int main(int argc, char * argv[])
{
    Application app(argc, argv);
    
    Builder builder(app);
    
    return 0;
}

// TODO Why?
// if (GL_BGR != parser.format() && GL_BGRA != parser.format())
//     image = image.rgbSwapped();



