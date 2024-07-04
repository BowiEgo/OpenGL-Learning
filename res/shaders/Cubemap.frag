#version 330 core
out vec4 FragColor;

in vec3 v_TexCoords;

uniform vec3 u_CameraPosition;
uniform samplerCube u_CubemapTexture;

void main()
{    
    vec3 envColor = texture(u_CubemapTexture, v_TexCoords).rgb;

    // envColor = envColor / (envColor + vec3(1.0));
    // envColor = pow(envColor, vec3(1.0/2.2)); 

    FragColor = vec4(envColor, 1.0);
}