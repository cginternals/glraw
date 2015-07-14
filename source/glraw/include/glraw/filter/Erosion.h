#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{
	class GLRAW_API Erosion : public AbstractFilter
	{
	public:
		Erosion(int kernelsize);
		Erosion(const QVariantMap& cfg);
		virtual ~Erosion() = default;

		virtual bool process(std::unique_ptr<Canvas> & imageData, AssetInformation & info) override;

	protected:

		void setUniforms(QOpenGLShaderProgram& program) override;

	private:

		int m_kernelsize;

		static int KernelSizeFromVariant(const QVariantMap& cfg);
	};

}