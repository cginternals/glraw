#version 330

uniform sampler2D image;
uniform int width;
uniform int height;

in vec2 texCoord;

layout(location = 0) out vec4 fragColor;

const int blurSize = 31;

void main()
{   
    vec2 texelSize = 1.0 / vec2(width, height);

    vec4 color = vec4(0.0);
    vec2 hlim = vec2(-blurSize * 0.5 + 0.5);

    for (int x = 0; x < blurSize; ++x) {
        for (int y = 0; y < blurSize; ++y) {
            vec2 offset = vec2(x, y);

            offset += hlim;
            offset *= texelSize;
            
            color += texture(image, texCoord + offset);
        }
    }

    fragColor = color / (blurSize * blurSize);
}
