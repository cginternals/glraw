#pragma once

#include <QStringList>
#include <QMap>
#include <QCommandLineParser>
#include <QScopedPointer>

#include <glraw/ConvertManager.h>


class QCoreApplication;

namespace glraw
{
    class FileWriter;
    class AbstractConverter;
}

struct CommandLineOption;

class Builder
{
public:
    using ConfigureMethod = bool (Builder::*)(const QString &);
    
    Builder();
    ~Builder();

    void process(const QCoreApplication & app);
    
protected:
    static QList<CommandLineOption> commandLineOptions();

protected:
    void initialize();
    
    bool help(const QString & name);
    bool outputPath(const QString & name);
    bool quiet(const QString & name);
    bool noSuffixes(const QString & name);
    bool format(const QString & name);
    bool type(const QString & name);
    bool compressedFormat(const QString & name);
    bool raw(const QString & name);
    bool shader(const QString & name);
	bool uniform(const QString & name);

	bool filter(const QString & name);
	bool input(const QString & name);

protected:

    bool configureShader();
    
    void showHelp() const;

	void addFilter();

protected:
    QCommandLineParser m_parser;
    QMap<QString, ConfigureMethod> m_configureMethods;

    QString m_shaderSource;
    QStringList m_uniformList;

	QStringList m_inputFiles;

	QString m_currentFilterName;
	QVariantMap m_currentFilterOptions;

    glraw::AbstractConverter * m_converter;
    glraw::FileWriter * m_writer;
    glraw::ConvertManager m_manager;

};
