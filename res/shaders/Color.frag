#version 330 core
out vec4 FragColor;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;
uniform float u_AmbientStrength;
uniform float u_SpecularStrength;
uniform float u_Shininess;

in vec3 v_Normal;
in vec3 v_FragPosition;

void main()
{
    // Ambient
    vec3 ambient = u_AmbientStrength * u_LightColor;

    // Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_LightPosition - v_FragPosition);

    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // Specular
    vec3 viewDirection = normalize(u_CameraPosition - v_FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Shininess);
    vec3 specular = spec * u_SpecularStrength * u_LightColor;

    vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
    FragColor = vec4(result, 1.0);
}