#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 lightSpaceMatrix;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

void main()
{
   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;
   
   TexCoords = aTexCoords;
   Normal = transpose(inverse(mat3(worldMatrix))) * aNormal;
   FragPos = vec3(worldMatrix * vec4(aPos, 1.0));
   FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}