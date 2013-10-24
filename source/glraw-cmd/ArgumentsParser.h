
#pragma once

#include <QStringList>

enum Format
{
    GL_RED
,   GL_RG
,   GL_RGB
,   GL_BGR
,   GL_RGBA
,   GL_BGRA
,   GL_STENCIL_INDEX
,   GL_DEPTH_COMPONENT
,   GL_DEPTH_STENCIL
};

enum Type
{
    GL_UNSIGNED_BYTE
,   GL_BYTE
,   GL_UNSIGNED_SHORT
,   GL_SHORT
,   GL_UNSIGNED_INT
,   GL_INT
,   GL_FLOAT
};

class ArgumentsParser
{
public:
	static const float s_invalidFloat;
	static const int s_invalidInt;

	ArgumentsParser(QStringList arguments);
	virtual ~ArgumentsParser();

    bool isValid() const;
    Format format() const;
    Type type() const;
    bool mirrorHorizontal() const;
    bool mirrorVertical() const;
    bool asBGR() const;
    int width() const;
    int height() const;
    float scale() const;
    float widthScale() const;
    float heightScale() const;
    Qt::AspectRatioMode aspectRatioMode() const;
    Qt::TransformationMode transformationMode() const;
    QString filePath() const;

protected:
	bool m_valid;
	Format m_format;
	Type m_type;
	bool m_mirrorHorizontal;
	bool m_mirrorVertical;
	bool m_asBGR;
	int m_width;
	int m_height;
	float m_scale;
	float m_widthScale;
	float m_heightScale;
	Qt::AspectRatioMode m_aspectRatioMode;
	Qt::TransformationMode m_transformationMode;
    QString m_filePath;

private:
    bool showManual(QStringList arguments);
    bool showVersion(QStringList arguments);

	bool parseTargetFormat(QStringList::const_iterator & argumentsIt);
	bool parseTargetType(QStringList::const_iterator & argumentsIt);
	void parseOptions(QStringList::const_iterator & argumentsIt);
	bool parseScalingOptions(QStringList::const_iterator & argumentsIt);
	void parseAspectRatioModes(QStringList::const_iterator & argumentsIt);
	void parseTransformationModes(QStringList::const_iterator & argumentsIt);
	void parseFilePath(QStringList::const_iterator & argumentsIt);

private:
	static float parseFloat(const QString & string);
	static int parseInt(const QString & string);

	static QMap<QString, Format> targetFormatsMap();
	static QMap<QString, Type> targetTypesMap();
	static QStringList options();
	static QStringList scalingOptions();
	static QMap<QString, Qt::AspectRatioMode> aspectRatioModesMap();
	static QMap<QString, Qt::TransformationMode> transformationModesMap();

};
