#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{
	class GLRAW_API Sharpening : public AbstractFilter
	{
	public:
		Sharpening(unsigned int size);
		Sharpening(const QVariantMap& cfg);
		virtual ~Sharpening() = default;

	protected:
		virtual void setUniforms(QOpenGLShaderProgram& program, unsigned int pass) override;
		virtual QString fragmentShaderSource(unsigned int pass) override;

	private:
		unsigned int m_size;
	};

}