#version 330 core

#define MAX_LIGHTS 10

in vec3 v_FragPosition;
in vec3 v_Normal;
in vec2 v_TexCoords;

out vec4 FragColor;

uniform sampler2D u_Texture_Diffuse1;
uniform sampler2D u_Texture_Diffuse2;
uniform sampler2D u_Texture_Diffuse3;
uniform sampler2D u_Texture_Specular1;
uniform sampler2D u_Texture_Specular2;
uniform sampler2D u_Texture_Normal1;

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

vec3 texturedDiffuse = texture(u_Texture_Diffuse1, v_TexCoords).rgb;
vec3 texturedSpecular = texture(u_Texture_Specular1, v_TexCoords).rgb;
vec3 textureNormal = texture(u_Texture_Normal1, v_TexCoords).rgb;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    if (!light.enable)
    {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    // Final result
    vec3 ambient  = light.ambient  * texturedDiffuse;
    vec3 diffuse  = light.diffuse  * diff * texturedDiffuse;
    vec3 specular = light.specular * spec * texturedSpecular;
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if (!light.enable)
    {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));
    // Final result
    vec3 ambient  = light.ambient  * texturedDiffuse;
    vec3 diffuse  = light.diffuse  * diff * texturedDiffuse;
    vec3 specular = light.specular * spec * texturedSpecular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if (!light.enable)
    {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(light.position - fragPos);

    // Intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));
    // Final result
    vec3 ambient  = light.ambient  * texturedDiffuse;
    vec3 diffuse  = light.diffuse  * diff * texturedDiffuse;
    vec3 specular = light.specular * spec * texturedSpecular;
    ambient  *= (attenuation * intensity);
    diffuse  *= (attenuation * intensity);
    specular *= (attenuation * intensity);
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 final = vec3(0.0);
    vec3 pointLights = vec3(0.0);

    vec3 viewDirection = normalize(u_CameraPosition - v_FragPosition);

    vec3 directionalLight = calcDirLight(u_DirectionalLight, v_Normal, viewDirection);
    vec3 spotight =         calcSpotLight(u_SpotLight, v_Normal, v_FragPosition, viewDirection);

    for(int i = 0; i < u_NR_PointLights; i++)
        pointLights += calcPointLight(u_PointLights[i], v_Normal, v_FragPosition, viewDirection);

    final = directionalLight + pointLights + spotight;
    FragColor = vec4(final, 1.0);
}