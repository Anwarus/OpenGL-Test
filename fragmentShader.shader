#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
    color = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), 0.2);
}