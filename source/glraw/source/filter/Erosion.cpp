#include <glraw/filter/Erosion.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform int M;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec2 img_size = vec2(1.0f)/textureSize(src, 0);

			dst = vec4(1.0f);

			for(int i=-M; i<= M;++i)
			{
				dst = min(dst, texture(src, v_uv+img_size*i));
			}
		} )";
	const int DefaultKernelSize = 1;
}

namespace glraw
{

	Erosion::Erosion(int kernelsize = DefaultKernelSize)
		: m_kernelsize(kernelsize)
	{
	}

	Erosion::Erosion(const QVariantMap& cfg)
		: Erosion(KernelSizeFromVariant(cfg))
	{
	}

	bool Erosion::process(std::unique_ptr<Canvas> & imageData, AssetInformation & info)
	{
		return renderShader(imageData, source);
	}

	void Erosion::setUniforms(QOpenGLShaderProgram& program)
	{
		program.setUniformValue("M", m_kernelsize);
	}


	int Erosion::KernelSizeFromVariant(const QVariantMap& cfg)
	{
		return cfg.value("kernelsize", { DefaultKernelSize }).toInt();
	}

}
