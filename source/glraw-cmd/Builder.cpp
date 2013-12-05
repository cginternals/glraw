
#include "Builder.h"

#include <iostream>

#include <QDebug>
#include <QCoreApplication>
#include <QCommandLineOption>

#include <glraw/MirrorEditor.h>
#include <glraw/ScaleEditor.h>
#include <glraw/RawFileWriter.h>
#include <glraw/GLRawFileWriter.h>
#include <glraw/RawConverter.h>

#include "CommandLineOption.h"
#include "Conversions.h"

namespace
{
    void messageHandler(QtMsgType type, const QMessageLogContext & context, const QString & message)
    {
        if (type == QtDebugMsg)
            return;

        std::cerr << message.toStdString() << std::endl;
    }
}

Builder::Builder()
:   m_converter(new glraw::RawConverter())
,   m_writer(new glraw::GLRawFileWriter())
,   m_manager(m_converter, m_writer)
{
    initialize();
}

Builder::~Builder()
{
}

QList<CommandLineOption> Builder::commandLineOptions()
{
    QList<CommandLineOption> options;
    
    options.append({
        QStringList() << "h" << "help",
        "Displays this help.",
        QString(),
        &Builder::help
    });
    
    options.append({
        QStringList() << "q" << "quiet",
        "Suppresses any output",
        QString(),
        &Builder::quiet
    });
    
    options.append({
        QStringList() << "n" << "no-suffixes",
        "Disables file suffixes.",
        QString(),
        &Builder::noSuffixes
    });

    options.append({
        QStringList() << "f" << "format",
        "Output format (default: GL_RGBA)",
        "format",
        &Builder::format
    });

    options.append({
        QStringList() << "t" << "type",
        "Output type (default: GL_UNSIGNED_BYTE)",
        "type",
        &Builder::type
    });
    
    options.append({
        QStringList() << "r" << "raw",
        "Writes files without header.",
        QString(),
        &Builder::raw
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
        "Sets the width in px.",
        "integer",
        &Builder::width
    });

    options.append({
        QStringList() << "height",
        "Sets the height in px.",
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
    m_parser.addVersionOption();

    m_parser.addPositionalArgument("source", "Source file with Qt-supported image format.");

    for (auto option : commandLineOptions())
    {
        m_parser.addOption(QCommandLineOption(
            option.names,
            option.description,
            option.valueName
        ));
        
        for (auto name : option.names)
        {
            m_configureMethods.insert(
                name,
                option.configureMethod
            );
        }
    }
}

void Builder::process(const QCoreApplication & app)
{
    if (app.arguments().size() == 1)
        m_parser.showHelp();
    
    m_parser.process(app);
    
    for (auto option : m_parser.optionNames())
    {
        if (!(this->*m_configureMethods.value(option))(option))
            return;
    }

    QStringList sources = m_parser.positionalArguments();
    
    if (sources.size() < 1)
    {
        qDebug() << "No source files passed in.";
        return;
    }
    
    for (auto source : sources)
        m_manager.process(source);
}

bool Builder::help(const QString & name)
{
   qDebug() << qPrintable(m_parser.helpText()) << R"(
Formats:          Types:                     Transformation Modes:
  GL_RED            GL_UNSIGNED_BYTE           nearest
  GL_BLUE           GL_BYTE                    linear
  GL_GREEN          GL_UNSIGNED_SHORT        
  GL_RG             GL_SHORT                 Aspect Ratio Modes:
  GL_RGB            GL_UNSIGNED_INT            IgnoreAspectRatio
  GL_BGR            GL_INT                     KeepAspectRatio
  GL_RGBA           GL_FLOAT                   KeepAspectRatioByExpanding
  GL_BGRA       
)";
    return false;
}

bool Builder::quiet(const QString & name)
{
    qInstallMessageHandler(messageHandler);
    return true;
}

bool Builder::noSuffixes(const QString & name)
{
    m_writer->setSuffixesEnabled(false);
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
    
    m_converter->setFormat(Conversions::stringToFormat(formatString));

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
    
    m_converter->setType(Conversions::stringToType(formatString));

    return true;
}

bool Builder::raw(const QString & name)
{
    auto writer = new glraw::RawFileWriter();
    writer->setSuffixesEnabled(m_writer->suffixesEnabled());
    
    m_writer = writer;
    m_manager.setWriter(m_writer);
    
    return true;
}

bool Builder::mirrorVertical(const QString & name)
{
    const QString editorName = "MirrorEditor";
    if (!editorExists(editorName))
        appendEditor(editorName, new glraw::MirrorEditor());
    
    auto e = editor<glraw::MirrorEditor>(editorName);
    
    e->setVertical(true);
    
    return true;
}

bool Builder::mirrorHorizontal(const QString & name)
{
    const QString editorName = "MirrorEditor";
    if (!editorExists(editorName))
        appendEditor(editorName, new glraw::MirrorEditor());
    
    auto e = editor<glraw::MirrorEditor>(editorName);
    
    e->setHorizontal(true);
    
    return true;
}

bool Builder::scale(const QString & name)
{
    QString scaleString = m_parser.value(name);
    
    bool ok;
    float scale = scaleString.toFloat(&ok);
    if (!ok)
    {
        qDebug() << scaleString << "isn't a float.";
        return false;
    }

    const QString editorName = "ScaleEditor";
    if (!editorExists(editorName))
        appendEditor(editorName, new glraw::ScaleEditor());
    
    auto e = editor<glraw::ScaleEditor>(editorName);

    e->setScale(scale);
    
    return true;
}

bool Builder::widthScale(const QString & name)
{
    QString widthScaleString = m_parser.value(name);
    
    bool ok;
    float widthScale = widthScaleString.toFloat(&ok);
    if (!ok)
    {
        qDebug() << widthScaleString << "isn't a float.";
        return false;
    }

    const QString editorName = "ScaleEditor";
    if (!editorExists(editorName))
        appendEditor(editorName, new glraw::ScaleEditor());
    
    auto e = editor<glraw::ScaleEditor>(editorName);

    e->setWidthScale(widthScale);
    
    return true;
}

bool Builder::heightScale(const QString & name)
{
    QString heighScaleString = m_parser.value(name);
    
    bool ok;
    float heightScale = heighScaleString.toFloat(&ok);
    if (!ok)
    {
        qDebug() << heighScaleString << "isn't a float.";
        return false;
    }

    const QString editorName = "ScaleEditor";
    if (!editorExists(editorName))
        appendEditor(editorName, new glraw::ScaleEditor());
    
    auto e = editor<glraw::ScaleEditor>(editorName);

    e->setHeightScale(heightScale);
    
    return true;
}

bool Builder::width(const QString & name)
{
    QString widthString = m_parser.value(name);
    
    bool ok;
    int width = widthString.toInt(&ok);
    if (!ok)
    {
        qDebug() << widthString << "isn't a int.";
        return false;
    }

    const QString editorName = "ScaleEditor";
    if (!editorExists(editorName))
        appendEditor(editorName, new glraw::ScaleEditor());
    
    auto e = editor<glraw::ScaleEditor>(editorName);

    e->setWidth(width);
    
    return true;
}

bool Builder::height(const QString & name)
{
    QString heightString = m_parser.value(name);
    
    bool ok;
    int height = heightString.toInt(&ok);
    if (!ok)
    {
        qDebug() << heightString << "isn't a int.";
        return false;
    }

    const QString editorName = "ScaleEditor";
    if (!editorExists(editorName))
        appendEditor(editorName, new glraw::ScaleEditor());
    
    auto e = editor<glraw::ScaleEditor>(editorName);

    e->setHeight(height);
    
    return true;
}

bool Builder::transformMode(const QString & name)
{
    QString modeString = m_parser.value(name);
    
    if (!Conversions::isTransformationMode(modeString))
    {
        qDebug() << qPrintable(modeString) << "is not a transformation mode.";
        return false;
    }

    const QString editorName = "ScaleEditor";
    if (!editorExists(editorName))
        appendEditor(editorName, new glraw::ScaleEditor());
    
    auto e = editor<glraw::ScaleEditor>(editorName);
    
    e->setTransformationMode(
        Conversions::stringToTransformationMode(modeString)
    );

    return true;
}

bool Builder::aspectRatioMode(const QString & name)
{
    QString modeString = m_parser.value(name);
    
    if (!Conversions::isAspectRatioMode(modeString))
    {
        qDebug() << qPrintable(modeString) << "is not a transformation mode.";
        return false;
    }

    const QString editorName = "ScaleEditor";
    if (!editorExists(editorName))
        appendEditor(editorName, new glraw::ScaleEditor());
    
    auto e = editor<glraw::ScaleEditor>(editorName);
    
    e->setAspectRatioMode(
        Conversions::stringToAspectRatioMode(modeString)
    );

    return true;
}

bool Builder::editorExists(const QString & key)
{
    return m_editors.contains(key);
}

void Builder::appendEditor(const QString & key, glraw::ImageEditorInterface * editor)
{
    m_manager.appendImageEditor(editor);
    m_editors.insert(key, editor);
}
