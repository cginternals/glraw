
#include "Builder.h"

#include <QDebug>
#include <QCoreApplication>
#include <QCommandLineOption>

#include <glraw/Enumerations.h>

#include "CommandLineOption.h"
#include "Conversions.h"

Builder::Builder(const QCoreApplication & app)
:   m_converter()
,   m_fileWriter()
,   m_manager(m_converter, m_fileWriter)
{
    initialize();
    processArguments(app);
}

Builder::~Builder()
{
}

QList<CommandLineOption> Builder::commandLineOptions()
{
    QList<CommandLineOption> options;

    options.append({
        QStringList() << "q" << "quiet",
        "Suppresses any output",
        QString(),
        &Builder::quiet
    });

    options.append({
        QStringList() << "f" << "format",
        "Output format (default: GL_RGBA)",
        "format",
        &Builder::format
    });

    options.append({
        QStringList() << "t" << "type",
        "Output type (default: GL_INT)",
        "type",
        &Builder::type
    });

    options.append({
        QStringList() << "mv" << "mirror-vertical",
        "Mirrors the image vertically.",
        QString(),
        &Builder::mirrorVertical
    });

    options.append({
        QStringList() << "mh" << "mirror-horizontal", 
        "Mirrors the image horizontally.",
        QString(),
        &Builder::mirrorHorizontal
    });

    options.append({
        QStringList() << "s" << "scale",
        "Scales the image.",
        "decimal",
        &Builder::scale
    });

    options.append({
        QStringList() << "ws" << "width-scale",
        "Scales the width.",
        "decimal",
        &Builder::widthScale
    });

    options.append({
        QStringList() << "hs" << "height-scale",
        "Scales the height.",
        "decimal",
        &Builder::heightScale
    });

    options.append({
        QStringList() << "width",
        "Sets the width.",
        "integer",
        &Builder::width
    });

    options.append({
        QStringList() << "height",
        "Sets the height.",
        "integer",
        &Builder::height
    });

    options.append({
        QStringList() << "transform-mode",
        "Transformation mode used for resizing        "
        "(default: nearest)",
        "mode",
        &Builder::transformMode
    });
    
    options.append({
        QStringList() << "aspect-ratio-mode",
        "Aspect ratio mode used for resizing          "
        "(default: IgnoreAspectRatio)",
        "mode",
        &Builder::aspectRatioMode
    });

    return options;
}

void Builder::initialize()
{
    m_parser.setApplicationDescription("Converts Qt supported images to an OpenGL compatible raw format.");
    m_parser.addHelpOption();
    m_parser.addVersionOption();

    m_parser.addPositionalArgument("source", "Source file with Qt-supported image format.");

    for (auto option : commandLineOptions())
    {
        m_parser.addOption(QCommandLineOption(
            option.names,
            option.description,
            option.valueName
        ));

        m_configureMethods.insert(
            option.names.first(),
            option.configureMethod
        );
    }
}

void Builder::processArguments(const QCoreApplication & app)
{
    m_parser.process(app);
    
    for (auto option : m_parser.optionNames())
    {
        if (!(this->*m_configureMethods.value(option))(option))
            return;
    }

    QStringList positionalArguments = m_parser.positionalArguments();

    if (positionalArguments.size() < 1)
    {
        qDebug() << "No source files passed in.";
        return;
    }

    QString source = positionalArguments.at(0);

    m_manager.process(source);
}

bool Builder::quiet(const QString & name)
{
    return true;
}

bool Builder::format(const QString & name)
{
    QString formatString = m_parser.value(name);
    
    if (!Conversions::isFormat(formatString))
    {
        qDebug() << qPrintable(formatString) << "is not a format.";
        return false;
    }
    
    m_converter.setFormat(Conversions::stringToFormat(formatString));

    return true;
}

bool Builder::type(const QString & name)
{
    QString formatString = m_parser.value(name);
    
    if (!Conversions::isType(formatString))
    {
        qDebug() << qPrintable(formatString) << "is not a type.";
        return false;
    }
    
    m_converter.setType(Conversions::stringToType(formatString));

    return true;
}

bool Builder::mirrorVertical(const QString & name)
{
    return true;
}

bool Builder::mirrorHorizontal(const QString & name)
{
    return true;
}

bool Builder::scale(const QString & name)
{
    return true;
}

bool Builder::widthScale(const QString & name)
{
    return true;
}

bool Builder::heightScale(const QString & name)
{
    return true;
}

bool Builder::width(const QString & name)
{
    return true;
}

bool Builder::height(const QString & name)
{
    return true;
}

bool Builder::transformMode(const QString & name)
{
    return true;
}

bool Builder::aspectRatioMode(const QString & name)
{
    return true;
}
