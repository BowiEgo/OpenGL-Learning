#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emmision;
    float shininess;
    float emmisionStrength;
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
uniform float u_Time;

in vec3 v_FragPosition;
in vec3 v_Normal;
in vec2 v_TexCoords;

out vec4 FragColor;

void main()
{
    // Ambient
    vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, v_TexCoords).rgb;

    // Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_Light.position - v_FragPosition);

    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, v_TexCoords).rgb;

    // Specular
    vec3 viewDirection = normalize(u_CameraPosition - v_FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * texture(u_Material.specular, v_TexCoords).rgb;

    // Emmision
    vec3 emmision = texture(u_Material.emmision, vec2(v_TexCoords.x, v_TexCoords.y + u_Time)).rgb;
    emmision *= (vec3(1.0) - step(0.1, texture(u_Material.specular, v_TexCoords).rgb));
    emmision *= u_Material.emmisionStrength;

    // Final result
    vec3 result = ambient + diffuse + specular + emmision;
    FragColor = vec4(result, 1.0);
}