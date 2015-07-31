#include <glraw/filter/AbstractKernel.h>

#include <QOpenGLShaderProgram>

namespace glraw
{

AbstractKernel::AbstractKernel(unsigned int size, float factor)
	: m_size(VerifySize(size))
	, m_factor(factor)
{
}

unsigned int AbstractKernel::numberOfPasses()
{
	return 2;
}

void AbstractKernel::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("size", m_size);
	if(pass == Pass::Second)
	{
		program.setUniformValue("factor", m_factor);
	}
}

QString AbstractKernel::fragmentShaderSource(unsigned int pass)
{
	switch(pass)
	{
	default:
		qCritical("Invalid pass used");

	case Pass::First:
		return firstShader();

	case Pass::Second:
		return secondShader();
	}
}

}