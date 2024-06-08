#version 330 core

in vec3 v_FragPosition;
in vec3 v_Normal;
in vec2 v_TexCoords;

out vec4 FragColor;

uniform float u_Near;
uniform float u_Far;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));    
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / u_Far;
    FragColor = vec4(vec3(depth), 1.0);
}