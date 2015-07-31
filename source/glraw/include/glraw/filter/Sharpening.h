#pragma once
#include <glraw/filter/AbstractKernel.h>

namespace glraw
{
	class GLRAW_API Sharpening : public AbstractKernel
	{
	public:
		Sharpening(unsigned int size, float factor);
		Sharpening(const QVariantMap& cfg);
		virtual ~Sharpening() = default;

	protected:
		virtual QString firstShader() const override;
		virtual QString secondShader() const override;
	};

}