#version 330

uniform sampler2D image;
uniform int width;
uniform int height;

in vec2 texCoord;

layout(location = 0) out vec4 fragColor;

void main()
{   
    vec4 color = texture(image, texCoord);

    float gray = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    fragColor = vec4(gray, gray, gray, color.a);
}
