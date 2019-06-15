#version 410
layout (location=0) in float yPosition;
layout (location=1) in float xPosition;
uniform float scale;
uniform float offset;
void main()
{
    vec4 position = (xPosition,
                     yPosition*scale + offset,
                     0.0f,
                     1.0f);
    gl_Position = position;
}
