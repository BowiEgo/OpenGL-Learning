#version 330 core
out vec4 FragColor;

uniform float u_Split_CoordX;

void main()
{
    float alpha = 1.0;
    if (gl_FragCoord.x > u_Split_CoordX)
        alpha = 0.0;

    FragColor = vec4(1.0, 1.0, 0.0, alpha);
}