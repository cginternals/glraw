
#pragma once

#include <QStringList>
#include <QMap>
#include <QCommandLineParser>

#include <glraw/ConvertManager.h>
#include <glraw/RawConverter.h>
#include <glraw/RawFileWriter.h>

class QCoreApplication;

namespace glraw
{
    class ImageEditorInterface;
}

struct CommandLineOption;

class Builder
{
public:
    Builder(const QCoreApplication & app);
    ~Builder();

protected:
    static QList<CommandLineOption> commandLineOptions();

protected:
    void initialize();
    void processArguments(const QCoreApplication & app);
    
    bool quiet(const QString & name);
    bool format(const QString & name);
    bool type(const QString & name);
    bool mirrorVertical(const QString & name);
    bool mirrorHorizontal(const QString & name);
    bool scale(const QString & name);
    bool widthScale(const QString & name);
    bool heightScale(const QString & name);
    bool width(const QString & name);
    bool height(const QString & name);
    bool transformMode(const QString & name);
    bool aspectRatioMode(const QString & name);
    
protected:
    bool editorExists(const QString & key);
    void appendEditor(const QString & key, glraw::ImageEditorInterface * editor);
    
    template <class Editor>
    Editor * editor(const QString & key);

protected:
    QCommandLineParser m_parser;
    QMap<QString, bool (Builder::*)(const QString &)> m_configureMethods;
    QMap<QString, glraw::ImageEditorInterface *> m_editors;

    glraw::RawConverter m_converter;
    glraw::RawFileWriter m_fileWriter;
    glraw::ConvertManager m_manager;

};

#include "Builder.hpp"
