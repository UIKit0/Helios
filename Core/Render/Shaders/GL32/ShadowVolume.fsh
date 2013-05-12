#version 150

uniform sampler2D UNIFORM_SAMPLER0;

out vec4 OUT_COLOR;

in vec4 vColor;
in vec2 vCoords;


void main()
{
    OUT_COLOR = vColor;
}
