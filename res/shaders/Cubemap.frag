#version 330 core
out vec4 FragColor;

in vec3 v_TexCoords;

uniform vec3 u_CameraPosition;
uniform samplerCube u_CubemapTexture;

void main()
{    
    FragColor = texture(u_CubemapTexture, v_TexCoords);
}