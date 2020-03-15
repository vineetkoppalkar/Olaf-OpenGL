#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 lightSpaceMatrix;

out vec2 vertexUV;
out vec4 FragPosLightSpace;

void main()
{
   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;
   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   vertexUV = aUV;

   vec3 FragPos = vec3(worldMatrix * vec4(aPos, 1.0));
   FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}