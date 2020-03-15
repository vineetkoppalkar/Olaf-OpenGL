#version 330 core

in vec2 vertexUV;
in vec4 FragPosLightSpace;

uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);
uniform sampler2D textureSampler;
uniform sampler2D shadowMap;

out vec4 FragColor;

float getShadowAmount(vec4 fragPosLightSpace)
{
    // Transform to normalized device coordinates
    // Returns the fragment's light-space position [-1, 1]
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform range to [0, 1]
    projCoords = projCoords * 0.5 + 0.5;

    // Closest depth value from the light's perspective
    float closestDepth = texture( shadowMap, projCoords.xy ).r;

    // Current depth fragment from the light's perspective
    float currentDepth = projCoords.z;

    // currentDepth > closestDepth ? in shadow : not in shadow
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
   vec3 textureColor = texture( textureSampler, vertexUV ).rgb;
   float shadowAmount = getShadowAmount(FragPosLightSpace);

   vec3 ambient = 0.3 * textureColor;

   vec3 lighting = (ambient + (1.0f - shadowAmount)) * textureColor;
   FragColor = vec4(lighting, 1.0);
   //FragColor = (ambient + (1.0f - shadowAmount)) *  vec4(vec3(textureColor), 1.0);
}