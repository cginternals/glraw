#include <glraw/FileNameSuffix.h>

#include <cassert>

#include <QMap>
#include <QRegExp>
#include <QStringList>

#include <glraw/S3TCExtensions.h>


namespace
{
	static const QMap<GLenum, QString> suffixesByFormat = 
	{
		{ GL_RED,   "r"    },
		{ GL_GREEN, "g"    },
		{ GL_BLUE,  "b"    },
		{ GL_RG,    "rg"   },
		{ GL_RGB,   "rgb"  },
		{ GL_BGR,   "bgr"  },
		{ GL_RGBA,  "rgba" },
		{ GL_BGRA,  "bgra" }
	};

	static const QMap<GLenum, QString> suffixesByType =
	{
		{ GL_UNSIGNED_BYTE,  "ub" },
		{ GL_BYTE,           "b"  },
		{ GL_UNSIGNED_SHORT, "us" },
		{ GL_SHORT,          "s"  },
		{ GL_UNSIGNED_INT,   "ui" },
		{ GL_INT,            "i"  },
		{ GL_FLOAT,          "f"  }

	#ifdef GL_ARB_texture_compression_rgtc
		,
		{ GL_COMPRESSED_RED_RGTC1,        "rgtc1-r"   },
		{ GL_COMPRESSED_SIGNED_RED_RGTC1, "rgtc1-sr"  },
		{ GL_COMPRESSED_RG_RGTC2,         "rgtc2-rg"  },
		{ GL_COMPRESSED_SIGNED_RG_RGTC2,  "rgtc2-srg" }
	#endif
	#ifdef GL_ARB_texture_compression_bptc
		,
		{ GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,         "bptc-rgba-unorm" },
		{ GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,   "bptc-rgb-sf"     },
		{ GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB, "bptc-rgb-uf"     }
	#endif
	#ifdef GLRAW_DXT // special treatment here - see S3TCExtensions.h
		,
		{ GL_COMPRESSED_RGB_S3TC_DXT1_EXT,  "dxt1-rgb"  },
		{ GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, "dxt1-rgba" },
		{ GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, "dxt3-rgba" },
		{ GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, "dxt5-rgba" }
	#endif
	};
}


namespace glraw
{

FileNameSuffix::FileNameSuffix(const QString & fileName)
: m_width (-1)
, m_height(-1)
, m_format(GL_INVALID_ENUM)
, m_type(GL_INVALID_ENUM)
, m_compressed(false)
{
	// check if either compressed or uncompressed (or unknown) format

	QRegExp regexp(R"(^.*\.(\d+)\.(\d+)\.(\w+)\.raw$)");
	if (regexp.exactMatch(fileName))
		m_compressed = true;
	else
	{
		regexp = QRegExp(R"(^.*\.(\d+)\.(\d+)\.(\w+)\.(\w+)\.raw$)");
		if (!regexp.exactMatch(fileName))
			return;
	}

	// retrieve intel from suffix parts

	QStringList parts = regexp.capturedTexts();

	bool ok;

	m_width = parts[1].toInt(&ok);
	assert(ok);

	m_height = parts[2].toInt(&ok);
	assert(ok);

	if (!m_compressed)
	{
		m_format = format(parts[3]);
		assert(m_format != GL_INVALID_ENUM);
	}

	m_type = type(parts[m_compressed ? 3 : 4]);
	assert(m_type != GL_INVALID_ENUM);
}

FileNameSuffix::FileNameSuffix(
	const int width, const int height, const GLenum format, const GLenum type)
: m_width(width)
, m_height(height)
, m_format(format)
, m_type(type)
, m_compressed(false)
{
	m_suffix = QString(".%1.%2.%3.%4")
		.arg(m_width).arg(m_height).arg(formatSuffix(m_format)).arg(typeSuffix(m_type));
}

FileNameSuffix::FileNameSuffix(
	const int width, const int height, const GLenum compressedType)
: m_width(width)
, m_height(height)
, m_format(GL_INVALID_ENUM)
, m_type(compressedType)
, m_compressed(true)
{
    m_suffix = QString(".%1.%2.%3")
        .arg(m_width).arg(m_height).arg(typeSuffix(m_type));
}

bool FileNameSuffix::isValid() const
{
	return m_width  != -1
		&& m_height != -1
		&& m_type   != GL_INVALID_ENUM
		&& (m_compressed || m_format != GL_INVALID_ENUM);
}

int FileNameSuffix::width() const
{
	return m_width;
}

int FileNameSuffix::height() const
{
	return m_height;
}

GLenum FileNameSuffix::type() const
{
	return m_type;
}

GLenum FileNameSuffix::format() const
{
	return m_format;
}

bool FileNameSuffix::compressed() const
{
	return m_compressed;
}

const QString & FileNameSuffix::get() const
{
	return m_suffix;
}

const QString FileNameSuffix::formatSuffix(const GLenum format)
{
	return suffixesByFormat.value(format, "");
}

GLenum FileNameSuffix::format(const QString & format)
{
	return suffixesByFormat.key(format.toLower(), GL_INVALID_ENUM);
}

const QString FileNameSuffix::typeSuffix(const GLenum type)
{
	return suffixesByType.value(type, "");
}

GLenum FileNameSuffix::type(const QString & type)
{
	return suffixesByType.key(type.toLower(), GL_INVALID_ENUM);
}

} // namespace glraw
