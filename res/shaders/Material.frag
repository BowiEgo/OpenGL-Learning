#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material u_Material;
uniform Light u_Light;

uniform vec3 u_CameraPosition;

in vec3 v_Normal;
in vec3 v_FragPosition;

out vec4 FragColor;

void main()
{
    // Ambient
    vec3 ambient = u_Material.ambient * u_Light.ambient;

    // Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_Light.position - v_FragPosition);

    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = (diff * u_Material.diffuse) * u_Light.diffuse;

    // Specular
    vec3 viewDirection = normalize(u_CameraPosition - v_FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);
    vec3 specular = (spec * u_Material.specular) * u_Light.specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}