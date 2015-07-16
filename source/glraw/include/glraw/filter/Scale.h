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

		virtual bool process(std::unique_ptr<Canvas> & imageData, AssetInformation & info) override;

	protected:

		void setUniforms(QOpenGLShaderProgram& program) override;

	private:

		ScaleMode m_mode;
		unsigned int m_width;
		unsigned int m_height;
		float m_scale;
		bool m_bilinear;


		static ScaleMode ModeFromVariant(const QVariantMap& cfg);
		static unsigned int WidthFromVariant(const QVariantMap& cfg);
		static unsigned int HeightFromVariant(const QVariantMap& cfg);
		static float ScaleFromVariant(const QVariantMap& cfg);
		static bool BilinearFromVariant(const QVariantMap& cfg);
	};

}