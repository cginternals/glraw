
#include <QMap>
#include <QDebug>
#include <QImage>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QStringList>
#include <QCoreApplication>

#include "Application.h"
#include "Builder.h"

int main(int argc, char * argv[])
{
    Application app(argc, argv);
    
    Builder builder;
    builder.process(app);
    
    return 0;
}
