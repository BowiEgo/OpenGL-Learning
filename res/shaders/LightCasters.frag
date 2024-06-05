#version 330 core
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

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

struct Light {
    int type;
    bool enable;
    vec3 position;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

uniform vec3 u_CameraPosition;
uniform float u_Time;

uniform TexturedMaterial u_Material;
uniform Light u_DirectionalLight;
uniform Light u_PointLight;
uniform Light u_SpotLight;

in vec3 v_FragPosition;
in vec3 v_Normal;
in vec2 v_TexCoords;

out vec4 FragColor;

vec3 texturedDiffuse = texture(u_Material.diffuse, v_TexCoords).rgb;
vec3 texturedSpecular = texture(u_Material.specular, v_TexCoords).rgb;

vec3 calcLight(Light light) {
    if (!light.enable)
        return vec3(0.0);

    // Direction
    vec3 direction;
    if (light.type == LIGHT_TYPE_DIRECTIONAL)
        direction = normalize(-light.direction);
    else if (light.type == LIGHT_TYPE_POINT || light.type == LIGHT_TYPE_SPOT)
        direction = normalize(light.position - v_FragPosition);

    // Intensity
    float intensity = 1.0;
    if (light.type == LIGHT_TYPE_SPOT)
    {
        float theta = dot(direction, normalize(-light.direction));
        float epsilon = light.cutOff - light.outerCutOff;
        intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    }

    // Ambient
    vec3 ambient = light.ambient * texturedDiffuse;

    // Diffuse
    vec3 normal = normalize(v_Normal);
    float diff = max(dot(normal, direction), 0.0);
    vec3 diffuse = light.diffuse * diff * texturedDiffuse;

    // Specular
    vec3 viewDirection = normalize(u_CameraPosition - v_FragPosition);
    vec3 reflectDirection = reflect(-direction, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);
    vec3 specular = light.specular * spec * texturedSpecular;

    // Attenuation
    float attenuation = 1.0;
    if (light.type == LIGHT_TYPE_POINT)
    {
        float distance    = length(light.position - v_FragPosition);
        attenuation = 1.0 / (light.constant + light.linear * distance + 
                            light.quadratic * (distance * distance));
    }

    return (ambient + diffuse + specular) * attenuation * intensity;
}

void main()
{
    vec3 directionalLight = calcLight(u_DirectionalLight);
    vec3 pointLight =       calcLight(u_PointLight);
    vec3 spotight =         calcLight(u_SpotLight);

    FragColor = vec4(directionalLight + pointLight + spotight, 1.0);
}