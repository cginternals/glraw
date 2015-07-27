#pragma once

#include <QVariant>
#include <QVector4D>
#include <QVector3D>

namespace glraw
{

class Lookup
{
public:
	Lookup() = default;
	virtual ~Lookup() = default;

protected:

	static float Get(const QString & key, float default_value, const QVariantMap& cfg)
	{
		return cfg.value(key, default_value).toFloat();
	}

	static unsigned int Get(const QString & key, unsigned int default_value, const QVariantMap& cfg)
	{
		return cfg.value(key, default_value).toInt();
	}

	static bool Get(const QString & key, bool default_value, const QVariantMap& cfg)
	{
		return cfg.value(key, default_value).toBool();
	}

	static float GetFactor(float default_value, const QVariantMap& cfg)
	{
		return Get("factor", default_value, cfg);
	}

	static float GetAmount(float default_value, const QVariantMap& cfg)
	{
		return Get("amount", default_value, cfg);
	}

	static unsigned int GetSize(unsigned int default_value, const QVariantMap& cfg)
	{
		return Get("size", default_value, cfg);
	}

	static QVector4D GetColor(const QVector4D& default_value, const QVariantMap& cfg)
	{
		QVector3D color = GetColor(default_value.toVector3D(), cfg);
		float a = cfg.value("a", { default_value.w() }).toFloat();

		return{ color, a };
	}

	static QVector3D GetColor(const QVector3D& default_value, const QVariantMap& cfg)
	{
		float r = cfg.value("r", { default_value.x() }).toFloat();
		float g = cfg.value("g", { default_value.y() }).toFloat();
		float b = cfg.value("b", { default_value.z() }).toFloat();

		return{ r, g, b };
	}

	static unsigned int VerifySize(unsigned int size)
	{
		if(size == 0)
		{
			qDebug("The minimum size is 1.");
			return 1;
		}

		return size;
	}
};

}