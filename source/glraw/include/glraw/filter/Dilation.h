#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{
	class GLRAW_API Dilation : public AbstractFilter
	{
	public:
		Dilation(int kernelsize);
		Dilation(const QVariantMap& cfg);
		virtual ~Dilation() = default;

		virtual bool process(std::unique_ptr<Canvas> & imageData, AssetInformation & info) override;

	protected:

		void setUniforms(QOpenGLShaderProgram& program) override;

	private:

		int m_kernelsize;

		static int KernelSizeFromVariant(const QVariantMap& cfg);
	};

}