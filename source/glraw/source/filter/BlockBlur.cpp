#include <glraw/filter/BlockBlur.h>

namespace
{
	const char* verticalShader = 
			R"(#version 150

			uniform sampler2D src;
			uniform int size;

			in vec2 v_uv;
			out vec4 fragColor;

			void main()
			{
				vec3 color = vec3(0.0);
				float offset = 1.0 / textureSize(src, 0).y;
				for (int i = -size; i <= size; ++i)
				{
					color += texture(src, v_uv + vec2(0.0, i * offset)).rgb;
				}
				fragColor = vec4(color / (2*size+1), 1.0);
			})";

	const char* horizontalShader =
		R"(#version 150

			uniform sampler2D src;
			uniform sampler2D buf;
			uniform float factor;
			uniform int size;

			in vec2 v_uv;
			out vec4 fragColor;

			void main()
			{
				vec3 color = vec3(0.0);
				float offset = 1.0 / textureSize(src, 0).x;
				for (int i = -size; i <= size; ++i)
				{
					color += texture(buf, v_uv + vec2(i * offset, 0.0)).xyz;
				}
				color = mix(texture(src, v_uv).rgb, color / (2*size+1), factor);
				fragColor = vec4(color, 1.0);
			})";

	const int DefaultSize = 1;
	const float DefaultFactor = 1.0f;
}

namespace glraw
{

BlockBlur::BlockBlur( unsigned int size = DefaultSize, float factor = DefaultFactor )
	: AbstractKernel(size, factor)
{
}

BlockBlur::BlockBlur(const QVariantMap& in)
	: BlockBlur(GetSize(DefaultSize, in), GetFactor(DefaultFactor, in))
{
}

QString BlockBlur::firstShader() const
{
	return verticalShader;
}

QString BlockBlur::secondShader() const
{
	return horizontalShader;
}

}