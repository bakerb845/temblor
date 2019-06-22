#version 410 
uniform vec4 color;
out vec4 color_out;
void main()
{
    color_out = color; //vec4(1.0, 0.0, 0.0, 1.0); //color; 
}
