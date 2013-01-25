#version 150

uniform sampler2D UNIFORM_SAMPLER0;

out vec4 OUT_FRAG;


in vec4 vColor;
in vec2 vCoords;


void main()
{
    OUT_FRAG =vec4((vColor.rgb + texture(UNIFORM_SAMPLER0, vCoords).rgb) * 0.5, 1.0);
}
