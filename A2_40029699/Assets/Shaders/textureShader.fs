#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform int calculateShadows = 1;
uniform int showTexture = 1;
uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);

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
    // Percentage-closer filtering (PCF)
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float  pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
    shadow /= 9.0;

    // Set shadow to 0 when outside the far_plane of light's frustum
    if (projCoords.z > 1.0)
    {
        shadow = 0.0;
	}
    return shadow;
}

void main()
{
   vec3 textureColor = texture( diffuseTexture, TexCoords ).rgb;
   if (showTexture == 0)
   {
        textureColor = color;
   }

   vec3 ambient = 0.05 * textureColor;

   float shadowAmount = 0.0;
   if (calculateShadows == 1)
   {
       shadowAmount = getShadowAmount(FragPosLightSpace);
   }

   vec3 lighting = (ambient + (1.0f - shadowAmount)) * textureColor;

   FragColor = vec4(lighting, 1.0);

}