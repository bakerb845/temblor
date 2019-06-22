#version 410
layout (location=0) in vec4 coord2d;
uniform float offset_x;
uniform float scale_x;
void main()
{
    float x = (coord2d.x + offset_x)*scale_x;
    //gl_Position = vec4((coord2d.x + offset_x)*scale_x, coord2d.y, 0.0, 1.0);
    gl_Position = vec4(x, coord2d.y, 0.0f, 1.0f);
}
