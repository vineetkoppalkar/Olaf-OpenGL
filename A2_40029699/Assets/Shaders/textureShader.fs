#version 330 core

in vec2 vertexUV;

uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);
uniform sampler2D textureSampler;

out vec4 FragColor;

void main()
{
   vec4 textureColor = texture( textureSampler, vertexUV );
   FragColor = textureColor;
}