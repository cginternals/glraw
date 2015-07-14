#pragma once
#include <glraw/filter/AbstractFilter.h>

namespace glraw
{

	class GLRAW_API Scale : public AbstractFilter
	{
	public:
		enum ScaleMode
		{
			Absolute = 0,
			Relative = 1,
			RatioX = 2,
			RatioY = 3
		};
		Scale(ScaleMode mode, int width, int height, float scale, bool bilinear);
		Scale(const QVariantMap& cfg);
		virtual ~Scale() = default;

		virtual bool process(std::unique_ptr<Canvas> & imageData, AssetInformation & info) override;

	protected:

		void setUniforms(QOpenGLShaderProgram& program) override;

	private:

		ScaleMode m_mode;
		int m_width;
		int m_height;
		float m_scale;
		bool m_bilinear;


		static ScaleMode ModeFromVariant(const QVariantMap& cfg);
		static int WidthFromVariant(const QVariantMap& cfg);
		static int HeightFromVariant(const QVariantMap& cfg);
		static float ScaleFromVariant(const QVariantMap& cfg);
		static bool BilinearFromVariant(const QVariantMap& cfg);
	};

}