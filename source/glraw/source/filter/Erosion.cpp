#include <glraw/filter/Erosion.h>

namespace
{
	const char * const verticalShader =
		R"(#version 150

		uniform sampler2D src;
		uniform int size;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec2 img_size = vec2(1.0f)/textureSize(src, 0);
			dst = vec4(1.0f);

			for(int i=-size; i <= size; ++i)
			{
				dst = min(dst, texture(src, v_uv+img_size*vec2(i,0)));
			}
		} )";

	const char * const horizontalShader =
		R"(#version 150

		uniform sampler2D src;
		uniform sampler2D buf;
		uniform int size;
		uniform float factor;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec2 img_size = vec2(1.0f)/textureSize(src, 0);
			dst = vec4(1.0f);

			for(int i=-size; i <= size; ++i)
			{
				dst = min(dst, texture(buf, v_uv+img_size*vec2(0,i)));
			}
			vec4 texel = texture(src, v_uv);
			dst = vec4(mix(texel.rgb, dst.rgb, factor), texel.a);
		} )";

	const unsigned int DefaultSize = 1;
	const float DefaultFactor = 1.0f;
}

namespace glraw
{

Erosion::Erosion(unsigned int size = DefaultSize, float factor = DefaultFactor)
	: AbstractKernel(size, factor)
{
}

Erosion::Erosion(const QVariantMap& cfg)
	: Erosion(GetSize(DefaultSize, cfg), GetFactor(DefaultFactor, cfg))
{
}

QString Erosion::firstShader() const
{
	return verticalShader;
}

QString Erosion::secondShader() const
{
	return horizontalShader;
}

}
