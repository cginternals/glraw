#include <glraw/filter/EdgeDetection.h>

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

			for(int i=-size; i<=size;++i)
			{
				dst += texture(src, v_uv + img_size*vec2(i,0));
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

			for(int i=-size; i<=size;++i)
			{
				dst += texture(buf, v_uv + img_size*vec2(0,i));
			}
			vec4 texel = texture(src, v_uv);
			vec3 weigthed = texel.rgb*(2*size+1)*(2*size+1);
			dst = vec4( (weigthed - dst.rgb) * factor, texel.a );
		} )";

	const unsigned int DefaultSize = 1;
	const float DefaultFactor = 1.0f;
}

namespace glraw
{

EdgeDetection::EdgeDetection(unsigned int size = DefaultSize, float factor = DefaultFactor)
	: AbstractKernel(size, factor)
{
}

EdgeDetection::EdgeDetection(const QVariantMap& cfg)
	: EdgeDetection(GetSize(DefaultSize, cfg), GetFactor(DefaultFactor, cfg))
{
}

QString EdgeDetection::firstShader() const
{
	return verticalShader;
}

QString EdgeDetection::secondShader() const
{
	return horizontalShader;
}

}
