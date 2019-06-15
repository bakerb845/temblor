#version 410
in float yPosition;
in float xPosition;
uniform float scale;
uniform float offset;
void main()
{
    vec4 position = vec4(xPosition, scale*yPosition + offset, 0.0, 1.0); //xPosition, yPosition, 0.0, 1.0);
    gl_Position = position;
}
