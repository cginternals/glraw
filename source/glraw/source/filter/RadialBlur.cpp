#include <glraw/filter/RadialBlur.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform vec2 pos;
		uniform float blur;
		uniform float bright;
		uniform int size;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec2 img_size = vec2(1.0f)/textureSize(src, 0);
			vec3 color = vec3(0.f);
			vec2 coords = v_uv+img_size*0.5-pos;
			
			for(int i=0;i<size;++i)
			{
				float scale = 1.0f - blur*(float(i)/(float(size)-1.f));
				color += texture(src, coords*scale + pos).rgb;
			}
			dst = vec4(color/(float(size))*bright,texture(src,v_uv).a);
		} )";

	const float DefaultX = 0.5f;
	const float DefaultY = 0.5f;
	const float DefaultBlur = 0.1f;
	const float DefaultBright = 1.0f;
	const int DefaultSize = 10;
}

namespace glraw
{

	RadialBlur::RadialBlur(float x = DefaultX, float y = DefaultY, float blur = DefaultBlur, float bright = DefaultBright, unsigned int size=DefaultSize)
	: m_x(x)
	, m_y(y)
	, m_blur(blur)
	, m_bright(bright)
	, m_size(VerifySize(size))
{
}

RadialBlur::RadialBlur(const QVariantMap& cfg)
	: RadialBlur(XFromVariant(cfg, DefaultX), YFromVariant(cfg, DefaultY), BlurFromVariant(cfg, DefaultBlur), BrightFromVariant(cfg, DefaultBright), SizeFromVariant(cfg, DefaultSize))
{
}

void RadialBlur::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
{
	program.setUniformValue("pos", QVector2D(m_x,m_y));
	program.setUniformValue("blur", m_blur);
	program.setUniformValue("bright", m_bright);
	program.setUniformValue("size", m_size);
}

QString RadialBlur::fragmentShaderSource(unsigned int pass)
{
	return source;
}

float RadialBlur::XFromVariant(const QVariantMap& cfg, float default_value)
{
	return cfg.value("x", { default_value }).toFloat();
}

float RadialBlur::YFromVariant(const QVariantMap& cfg, float default_value)
{
	return cfg.value("y", { default_value }).toFloat();
}

float RadialBlur::BlurFromVariant(const QVariantMap& cfg, float default_value)
{
	return cfg.value("blur", { default_value }).toFloat();
}

float RadialBlur::BrightFromVariant(const QVariantMap& cfg, float default_value)
{
	return cfg.value("bright", { default_value }).toFloat();
}

}
