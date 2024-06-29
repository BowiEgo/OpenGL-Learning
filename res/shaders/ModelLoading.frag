#version 330 core

#define MAX_LIGHTS 10

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 v_FragPosition;
in vec3 v_Normal;
in vec2 v_TexCoords;
in vec4 v_FragPosLightSpace;
in mat3 v_TBN;
in vec3 v_TangentCamPos;
in vec3 v_TangentFragPos;

in VS_OUT {
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D u_Texture_Diffuse1;
uniform sampler2D u_Texture_Diffuse2;
uniform sampler2D u_Texture_Diffuse3;
uniform sampler2D u_Texture_Specular1;
uniform sampler2D u_Texture_Specular2;
uniform sampler2D u_Texture_Normal1;
uniform sampler2D u_Texture_Height1;
uniform sampler2D u_Texture_ShadowMap1;
uniform samplerCube u_Texture_CubeShadowMap1;
uniform float u_Far;
uniform bool u_Shadow_Enabled;
uniform bool u_NormalMapping_Enabled;
uniform bool u_ParallaxMapping_Enabled;
uniform float u_Height_Scale;

uniform samplerCube u_Texture_Environment;
uniform bool u_Is_EnvironmentTexture_Valid;
uniform float u_Env_Reflective_Rate;
uniform float u_Env_Refractive_Rate;
uniform float u_Env_Refractive_Index;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct TexturedMaterial {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emmision;
    float shininess;
};

struct DirLight {
    bool enable;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    bool enable;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    bool enable;
    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
};

uniform vec3 u_CameraPosition;
uniform float u_Time;

uniform TexturedMaterial u_Material;
uniform DirLight u_DirectionalLight;
uniform int u_NR_PointLights;
uniform PointLight u_PointLights[MAX_LIGHTS];
uniform SpotLight u_SpotLight;

uniform bool u_Discard_Transparent;
uniform bool u_Is_Opaque;

uniform bool u_Blinn_Enabled;

uniform float u_Split_CoordX;
uniform float u_Exposure;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    const float minLayers = 8;
    const float maxLayers = 150;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * u_Height_Scale; 
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = 1.0 - texture(u_Texture_Height1, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = 1.0 - texture(u_Texture_Height1, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(u_Texture_Height1, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(u_Texture_ShadowMap1, projCoords.xy).r;
    float currentDepth = projCoords.z;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_Texture_ShadowMap1, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_Texture_ShadowMap1, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

float PointShadowCalculation(vec3 fragPos, vec3 lightPos, float viewDistance)
{
    vec3 fragToLight = fragPos - lightPos; 
    float currentDepth = length(fragToLight);

    vec3 sampleOffsetDirections[20] = vec3[]
    (
      vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
      vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
      vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
      vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float diskRadius = (1.0 + (viewDistance / u_Far)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(u_Texture_CubeShadowMap1, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= u_Far;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 texCoords)
{
    if (!light.enable)
    {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    float spec;
    if (u_Blinn_Enabled)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), u_Material.shininess * 2.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    }

    vec3 ambient  = light.ambient  * texture(u_Texture_Diffuse1, texCoords).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(u_Texture_Diffuse1, texCoords).rgb;
    vec3 specular = light.specular * spec * texture(u_Texture_Specular1, texCoords).rgb;
    // shadow
    float shadow = 0.0;
    if (u_Shadow_Enabled)
        shadow = ShadowCalculation(v_FragPosLightSpace, normal, lightDir);
 
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float viewDistance, vec2 texCoords)
{
    if (!light.enable)
    {
        return vec3(0.0);
    }

    vec3 lightPos = light.position;
    if (u_NormalMapping_Enabled)
        lightPos = v_TBN * lightPos;

    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    float spec;
    if (u_Blinn_Enabled)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), u_Material.shininess * 2.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    }

    float distance    = length(lightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient  * texture(u_Texture_Diffuse1, texCoords).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(u_Texture_Diffuse1, texCoords).rgb;
    vec3 specular = light.specular * spec * texture(u_Texture_Specular1, texCoords).rgb;
    // shadow
    float shadow = 0.0;
    if (u_Shadow_Enabled)
        shadow = PointShadowCalculation(fragPos, lightPos, viewDistance);

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (1.0 - shadow * attenuation) * (ambient + (diffuse + specular)) * light.color;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords)
{
    if (!light.enable)
    {
        return vec3(0.0);
    }

    vec3 lightPos = light.position;
    if (u_NormalMapping_Enabled)
        lightPos = v_TBN * lightPos;

    vec3 lightDir = normalize(lightPos - fragPos);

    // Intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, lightDir), 0.0);

    float spec;
    if (u_Blinn_Enabled)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), u_Material.shininess * 2.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    }

    float distance    = length(lightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient  * texture(u_Texture_Diffuse1, texCoords).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(u_Texture_Diffuse1, texCoords).rgb;
    vec3 specular = light.specular * spec * texture(u_Texture_Specular1, texCoords).rgb;

    ambient  *= (attenuation * intensity);
    diffuse  *= (attenuation * intensity);
    specular *= (attenuation * intensity);
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 final = vec3(0.0);
    vec3 pointLights = vec3(0.0);
    vec2 texCoords = v_TexCoords;
    vec3 normal = v_Normal;
    vec3 camPos = u_CameraPosition;
    vec3 fragPos = v_FragPosition;
    vec3 viewDirection;
    float viewDistance;

    if (u_ParallaxMapping_Enabled)
    {
        texCoords = ParallaxMapping(texCoords, normalize(v_TangentCamPos - v_TangentFragPos));
        if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
            discard;
    }

    if (u_NormalMapping_Enabled)
    {
        vec3 texNormal;
        texNormal = texture(u_Texture_Normal1, texCoords).rgb;
        texNormal = normalize(texNormal * 2.0 - 1.0);

        if (texNormal != vec3(0.0))
            normal = texNormal;

        camPos = v_TangentCamPos;
        fragPos = v_TangentFragPos;
    }

    viewDirection = normalize(camPos - fragPos);
    viewDistance = length(camPos - fragPos);

    // Environment mapping
    if (u_Is_EnvironmentTexture_Valid)
    {
        float ratio = 1.00 / u_Env_Refractive_Index;
        vec3 I = -viewDirection;
        vec3 reflectDir = reflect(I, normalize(normal));
        vec3 refractDir = refract(I, normalize(normal), ratio);

        vec3 reflection = texture(u_Texture_Height1, texCoords).rgb * texture(u_Texture_Environment, reflectDir).rgb;
        vec3 refraction = texture(u_Texture_Environment, refractDir).rgb;

        final += mix(vec3(0.0), reflection, u_Env_Reflective_Rate);
        final += mix(vec3(0.0), refraction, u_Env_Refractive_Rate);
    }

    // Lighting
    vec3 directionalLight = calcDirLight(u_DirectionalLight, normal, viewDirection, texCoords);
    vec3 spotight =         calcSpotLight(u_SpotLight, normal, fragPos, viewDirection, texCoords);

    for(int i = 0; i < u_NR_PointLights; i++)
        pointLights += calcPointLight(u_PointLights[i], normal, fragPos, viewDirection, viewDistance, texCoords);

    vec3 light = directionalLight + pointLights + spotight;

    final += light;
    // if (gl_FragCoord.x < u_Split_CoordX)
        // final += directionalLight + pointLights + spotight;

    // Alpha
    float alpha = texture(u_Texture_Diffuse1, texCoords).a;
    if (u_Discard_Transparent && alpha < 0.1)
        discard;

    if (u_Is_Opaque)
        alpha = 1.0;

    // final
    FragColor = vec4(final, alpha);

    BrightColor = vec4(vec3(0.0), 1.0);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
    {
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
}