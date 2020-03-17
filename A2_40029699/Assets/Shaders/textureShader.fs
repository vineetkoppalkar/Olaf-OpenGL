#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform vec3 lightPos;

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

    // Calculate bias
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // Check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
   vec3 textureColor = texture( diffuseTexture, TexCoords ).rgb;

   vec3 ambient = 0.3 * textureColor;
   float shadowAmount = getShadowAmount(FragPosLightSpace);
   vec3 lighting = (ambient + (1.0f - shadowAmount)) * textureColor;

   FragColor = vec4(lighting, 1.0);


   //FragColor = (ambient + (1.0f - shadowAmount)) *  vec4(vec3(textureColor), 1.0);
}