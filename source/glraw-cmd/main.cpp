
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
#include <glraw/MirrorEditor.h>
#include <glraw/ScaleEditor.h>

#include "ArgumentsParser.h"

int main(int argc, char * argv[])
{
    QCoreApplication a(argc, argv);
    
    ArgumentsParser parser(a.arguments());
    
    if (!parser.isValid())
        return 0;
    
    glraw::MirrorEditor mirrorEditor(parser.mirrorHorizontal(), parser.mirrorVertical());
    glraw::ScaleEditor scaleEditor;
    scaleEditor.setScale(0.5f);
    
    glraw::RawConverter converter;
    glraw::RawFileWriter writer;
    
    glraw::ConvertManager manager(converter, writer);
    manager.appendImageEditor(&mirrorEditor);
    manager.appendImageEditor(&scaleEditor);
    
    if (!manager.process(parser.filePath()))
        return 0;
    
    return 0;
}

// TODO Why?
// if (GL_BGR != parser.format() && GL_BGRA != parser.format())
//     image = image.rgbSwapped();



