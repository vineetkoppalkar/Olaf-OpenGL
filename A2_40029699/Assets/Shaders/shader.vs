#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

out vec3 Normal;
out vec3 FragPos;

void main()
{
   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;
   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);

   Normal = mat3(worldMatrix) * aNormal;
   FragPos = vec3(worldMatrix * vec4(aPos, 1.0));
};