
#include "ArgumentsParser.h"

#include <iostream>

#include <QMap>
#include <QtDebug>
#include <QPair>

#include <version.h>

void quietMessageHandler(QtMsgType type, const QMessageLogContext & /*context*/, const QString & msg)
{
    if (type == QtDebugMsg)
        return;

    std::cout << msg.toStdString() << std::endl;
}

const float ArgumentsParser::s_invalidFloat = -1.f;
const int ArgumentsParser::s_invalidInt = -1.f;

ArgumentsParser::ArgumentsParser(QStringList arguments)
:   m_valid(false)
,   m_mirrorHorizontal(false)
,   m_mirrorVertical(false)
,   m_asBGR(false)
,   m_width(s_invalidInt)
,   m_height(s_invalidInt)
,   m_scale(s_invalidFloat)
,   m_widthScale(s_invalidFloat)
,   m_heightScale(s_invalidFloat)
{
    if (showManual(arguments))
        return;

    if (showVersion(arguments))
        return;

    if (arguments.size() < 4)
    {
        qWarning() << "Not enough arguments passed. See -h for help.";
        return;
    }

	auto it = ++arguments.constBegin();

	if (!parseTargetFormat(it))
        return;

    if (!parseTargetType(it))
        return;

	parseOptions(it);

	if (!parseScalingOptions(it))
        return;

	parseFilePath(it);

    m_valid = true;
}

ArgumentsParser::~ArgumentsParser()
{  
}

bool ArgumentsParser::isValid() const
{
    return m_valid;
}

Format ArgumentsParser::format() const
{
    return m_format;
}

Type ArgumentsParser::type() const
{
    return m_type;
}

bool ArgumentsParser::mirrorHorizontal() const
{
    return m_mirrorHorizontal;
}

bool ArgumentsParser::mirrorVertical() const
{
    return m_mirrorVertical;
}

bool ArgumentsParser::asBGR() const
{
    return m_asBGR;
}

int ArgumentsParser::width() const
{
    return m_width;
}

int ArgumentsParser::height() const
{
    return m_height;
}

float ArgumentsParser::scale() const
{
    return m_scale;
}

float ArgumentsParser::widthScale() const
{
    return m_widthScale;
}

float ArgumentsParser::heightScale() const
{
    return m_heightScale;
}

Qt::AspectRatioMode ArgumentsParser::aspectRatioMode() const
{
    return m_aspectRatioMode;
}

Qt::TransformationMode ArgumentsParser::transformationMode() const
{
    return m_transformationMode;
}

QString ArgumentsParser::filePath() const
{
    return m_filePath;
}

bool ArgumentsParser::showManual(QStringList arguments)
{
    QStringList manualOptions;
    manualOptions << "--help" << "-h" << "/?";

    if (!(arguments.size() == 1 || manualOptions.contains(arguments.at(1))))
        return false;

    qDebug() << "";
    qDebug() << GLRAW_PROJECT_NAME << GLRAW_VERSION << "by" << GLRAW_AUTHOR_ORGANIZATION;
    qDebug() << "Converts an input image to an OpenGL compatible format.";
    qDebug() << "";
    qDebug() << "Usage:";
    qDebug() << "glraw <Target-Format> <Target-Type>";
    qDebug() << "      [<Options>]";
    qDebug() << "      [<ScalingOptions> [<AspectRatioMode>] [<TransformationMode>]]";
    qDebug() << "      <Input-FilePath>";
    qDebug() << "";

    qDebug() << "<Target-Formats>:";
    qDebug().nospace() << "  " << qPrintable(QStringList(targetFormatsMap().keys()).join("\n  "));
    qDebug() << "";

    qDebug() << "<Target-Types>:";
    qDebug().nospace() << "  " << qPrintable(QStringList(targetTypesMap().keys()).join("\n  "));
    qDebug() << "";

    QStringList options = this->options();
    qDebug() << "<Options>:";
    for (int i = 0; i < options.size(); i += 2)
    {
        qDebug().nospace() << "  " << qPrintable(options.at(i)) << " (" << qPrintable(options.at(i + 1)) << ")";  
    }
    qDebug() << "";

    QStringList scalingOptions = this->scalingOptions();
    qDebug() << "<ScalingOptions>:";
    for (int i = 0; i < scalingOptions.size(); i += 2)
    {
        qDebug().nospace() << "  " << qPrintable(scalingOptions[i]) << " (" << qPrintable(scalingOptions[i + 1]) << ")";    
    }
    qDebug() << "";

    QMap<QString, Qt::AspectRatioMode> aspectRatioModesMap = this->aspectRatioModesMap();
    qDebug() << "<AspectRatioMode>:";
    QStringList iar = aspectRatioModesMap.keys(Qt::IgnoreAspectRatio);
    qDebug().nospace() << "  " << qPrintable(iar[0]) << " (" << qPrintable(iar[1]) << ") (default)";
    QStringList kar = aspectRatioModesMap.keys(Qt::KeepAspectRatio);
    qDebug().nospace() << "  " << qPrintable(kar[0]) << " (" << qPrintable(kar[1]) << ")";
    QStringList karbe = aspectRatioModesMap.keys(Qt::KeepAspectRatioByExpanding);
    qDebug().nospace() << "  " << qPrintable(karbe[0]) << " (" << qPrintable(karbe[1]) << ")";
    qDebug() << "";

    QMap<QString, Qt::TransformationMode> transformationModesMap = this->transformationModesMap();
    qDebug() << "<TransformationMode>:";
    QStringList bilinear = transformationModesMap.keys(Qt::SmoothTransformation);
    qDebug().nospace() << "  " << qPrintable(bilinear[0]) << " (" << qPrintable(bilinear[1]) << ") (default)";
    QStringList nearest = transformationModesMap.keys(Qt::FastTransformation);
    qDebug().nospace() << "  " << qPrintable(nearest[0]) << " (" << qPrintable(nearest[1]) << ")";

    return true;
}

bool ArgumentsParser::showVersion(QStringList arguments)
{
    QStringList options;
    options << "--version" << "-v";

    if (!options.contains(arguments.at(1)))
        return false;

    qDebug() << GLRAW_PROJECT_NAME << GLRAW_VERSION;
    qDebug() << "";

    return true;
}

bool ArgumentsParser::parseTargetFormat(QStringList::const_iterator & argumentsIt)
{
	QMap<QString, Format> formats = targetFormatsMap();

    if (!formats.contains(*argumentsIt))
    {
    	qWarning() << "Unknown Target-Format" << *argumentsIt << ".";
    	return false;
    }

    m_format = formats[*argumentsIt];
    ++argumentsIt;

    return true;
}

bool ArgumentsParser::parseTargetType(QStringList::const_iterator & argumentsIt)
{
	QMap<QString, Type> types = targetTypesMap();

    if(!types.contains(*argumentsIt))
    {
        qWarning() << "Unknown Target-Type" << *argumentsIt << ".";
        return false;
    }

   	m_type = types[*argumentsIt];
    ++argumentsIt;

   	return true;
}

void ArgumentsParser::parseOptions(QStringList::const_iterator & argumentsIt)
{	
	QStringList options = this->options();

    static const int option_mh  = 0;
    static const int option_mv  = 2;
    static const int option_bgr = 4;
    static const int option_q   = 6;

    while (options.contains(*argumentsIt)) 
    {
    	m_mirrorHorizontal = *argumentsIt == options[option_mh]  || *argumentsIt == options[option_mh + 1];
        m_mirrorVertical   = *argumentsIt == options[option_mv]  || *argumentsIt == options[option_mv + 1];
        m_asBGR            = *argumentsIt == options[option_bgr] || *argumentsIt == options[option_bgr + 1];

        if (*argumentsIt == options[option_q] || *argumentsIt == options[option_q + 1])
            qInstallMessageHandler(quietMessageHandler);

        ++argumentsIt;
    }
}

bool ArgumentsParser::parseScalingOptions(QStringList::const_iterator & argumentsIt)
{
	QStringList options = scalingOptions();

    static const int option_w = 0;
    static const int option_h = 2;
    static const int option_s = 4;

    while (options.contains(*argumentsIt))
    {
        if(*argumentsIt == options[option_s] || *argumentsIt == options[option_s + 1])
        {
            ++argumentsIt;
            m_scale = parseFloat(*argumentsIt);

            if (m_scale == s_invalidFloat)
            {
                qWarning() << *argumentsIt << " is not a float.";
                return false;
            }
        }

        if(*argumentsIt == options[option_w] || *argumentsIt == options[option_w + 1])
        {
            ++argumentsIt;

            m_width = parseInt(*argumentsIt);
            if (m_width == s_invalidInt)
            {
                m_widthScale = parseFloat(*argumentsIt);
                if (m_widthScale == s_invalidFloat)
                {
                    qWarning() << *argumentsIt << " is neither a float nor an integer.";
                    return false;  
                }
            }
        }

        if(*argumentsIt == options[option_h] || *argumentsIt == options[option_h + 1])
        {
            ++argumentsIt;

            m_height = parseInt(*argumentsIt);
            if (m_height == s_invalidInt)
            {
                m_heightScale = parseFloat(*argumentsIt);
                if (m_widthScale == s_invalidFloat)
                {
                    qWarning() << *argumentsIt << " is neither a float nor an integer.";
                    return false;  
                }
            }
        }

        ++argumentsIt;
    }

    parseAspectRatioModes(argumentsIt);
    parseTransformationModes(argumentsIt);

    return true;
}

void ArgumentsParser::parseAspectRatioModes(QStringList::const_iterator & argumentsIt)
{
    QMap<QString, Qt::AspectRatioMode> modes = aspectRatioModesMap();

    if (!modes.contains(*argumentsIt))
    {
        m_aspectRatioMode = Qt::IgnoreAspectRatio;
        return;
    }

    m_aspectRatioMode = modes[*argumentsIt];
    ++argumentsIt;
}

void ArgumentsParser::parseTransformationModes(QStringList::const_iterator & argumentsIt)
{
    QMap<QString, Qt::TransformationMode> modes = transformationModesMap();

    if (!modes.contains(*argumentsIt))
    {
        m_transformationMode = Qt::FastTransformation;
        return;
    }

    m_transformationMode = modes[*argumentsIt];
    ++argumentsIt;
}

void ArgumentsParser::parseFilePath(QStringList::const_iterator & argumentsIt)
{
    m_filePath = *argumentsIt;
}

float ArgumentsParser::parseFloat(const QString & string)
{
	bool ok;
	float parsedValue = string.toFloat(&ok);

	if (!ok)
	{
		qWarning() << string << " is no float.";
		return s_invalidFloat;
	}

	if (parsedValue <= 0)
	{
		qWarning() << string << " is smaller or equal zero.";
		return s_invalidFloat;
	}

	return parsedValue;
}

int ArgumentsParser::parseInt(const QString & string)
{
	bool ok;
	int parsedValue = string.toInt(&ok);

	if (!ok)
	{
		qWarning() << string << " is no int.";
		return s_invalidInt;
	}

	if (parsedValue <= 0)
	{
		qWarning() << string << " is smaller or equal zero.";
		return s_invalidInt;
	}

	return parsedValue;
}

QMap<QString, Format> ArgumentsParser::targetFormatsMap()
{
    QMap<QString, Format> formats;
    formats["GL_RED"] = GL_RED;
    formats["GL_RG"] = GL_RG;
    formats["GL_RGB"] = GL_RGB;
    formats["GL_BGR"] = GL_BGR;
    formats["GL_RGBA"] = GL_RGBA;
    formats["GL_BGRA"] = GL_BGRA;

    return formats;
}

QMap<QString, Type> ArgumentsParser::targetTypesMap()
{
    QMap<QString, Type> types;
    types["GL_UNSIGNED_BYTE"] = GL_UNSIGNED_BYTE;
    types["GL_BYTE"] = GL_BYTE;
    types["GL_UNSIGNED_SHORT"] = GL_UNSIGNED_SHORT;
    types["GL_SHORT"] = GL_SHORT;
    types["GL_UNSIGNED_INT"] = GL_UNSIGNED_INT;
    types["GL_INT"] = GL_INT;
    types["GL_FLOAT"] = GL_FLOAT;

    return types;
}

QStringList ArgumentsParser::options()
{
    QStringList options;
    options << "--mirror-horizontal"  << "-mh" ;
    options << "--mirror-vertical"    << "-mv" ;
    options << "--as-bgr"             << "-bgr";
    options << "--quiet"              << "-q"  ;

    return options;
}

QStringList ArgumentsParser::scalingOptions()
{
    QStringList options;
    options << "--width"  << "-w";
    options << "--height" << "-h";
    options << "--scale"  << "-s";

    return options;
}

QMap<QString, Qt::AspectRatioMode> ArgumentsParser::aspectRatioModesMap()
{
    QMap<QString, Qt::AspectRatioMode> modes;
    modes["--ignore-aspect-ratio"] = Qt::IgnoreAspectRatio;
    modes["-iar"] = Qt::IgnoreAspectRatio;
    modes["--keep-aspect-ratio"] = Qt::KeepAspectRatio;
    modes["-kar"] = Qt::KeepAspectRatio;
    modes["--keep-aspect-ratio-by-expanding"] = Qt::KeepAspectRatioByExpanding;
    modes["-karbe"] = Qt::KeepAspectRatioByExpanding;

    return modes;
}

QMap<QString, Qt::TransformationMode> ArgumentsParser::transformationModesMap()
{
    QMap<QString, Qt::TransformationMode> modes;
    modes["--nearest"] = Qt::FastTransformation;
    modes["-n"] = Qt::FastTransformation;
    modes["--bilinear"] = Qt::SmoothTransformation;
    modes["-b"] = Qt::SmoothTransformation;

    return modes;
}
