#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

	class GLRAW_API Scale : public AbstractFilter
	{
	public:
		enum class ScaleMode : int
		{
			Absolute,
			Relative,
			RatioX,
			RatioY,

			Default = Relative
		};
		Scale(ScaleMode mode, unsigned int width, unsigned int height, float scale, bool bilinear);
		Scale(const QVariantMap& cfg);
		virtual ~Scale() = default;

	protected:

		virtual void updateAssetInformation(AssetInformation & info) override;

		virtual QString fragmentShaderSource(unsigned int pass) override;
		virtual int createWorkingTexture(unsigned int prototype) override;
		virtual void bindTexture(unsigned int unit, unsigned int tex) override;

	private:

		ScaleMode m_mode;
		unsigned int m_width;
		unsigned int m_height;
		float m_scale;
		bool m_bilinear;

		int out_width;
		int out_height;

		static ScaleMode ModeFromVariant(const QVariantMap& cfg);
	};

}