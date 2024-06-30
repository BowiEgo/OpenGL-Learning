#version 330 core

#define MAX_LIGHTS 32

out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D g_Position;
uniform sampler2D g_Normal;
uniform sampler2D g_AlbedoSpec;

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
uniform int u_NR_PointLights;
uniform PointLight u_PointLights[MAX_LIGHTS];

uniform vec3 u_CameraPosition;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float viewDistance, vec2 texCoords, vec3 Diffuse, float Specular)
{
    if (!light.enable)
    {
        return vec3(0.0);
    }

    vec3 lightPos = light.position;

    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    float spec;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0 * 2.0);

    float distance    = length(lightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient * Diffuse;
    vec3 diffuse  = light.diffuse  * diff * Diffuse;
    vec3 specular = light.specular * spec * Specular;
    // shadow
    float shadow = 0.0;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (1.0 - shadow * attenuation) * (ambient + (diffuse + specular)) * light.color;
}

void main()
{
    vec3 fragPos = texture(g_Position, v_TexCoords).rgb;
    vec3 normal = texture(g_Normal, v_TexCoords).rgb;
    vec3 albedo = texture(g_AlbedoSpec, v_TexCoords).rgb;
    float specular = texture(g_AlbedoSpec, v_TexCoords).a;

    vec3 final = vec3(0.0);
    vec3 pointLights = vec3(0.0);
    vec2 texCoords = v_TexCoords;
    vec3 camPos = u_CameraPosition;
    vec3 viewDirection;
    float viewDistance;

    viewDirection = normalize(camPos - fragPos);
    viewDistance = length(camPos - fragPos);

    for(int i = 0; i < u_NR_PointLights; i++)
        pointLights += calcPointLight(u_PointLights[i], normal, fragPos, viewDirection, viewDistance, texCoords, albedo, specular);

    vec3 light = pointLights;

    final += light;

    FragColor = vec4(final, 1.0);
} 