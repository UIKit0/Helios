#version 150

uniform sampler2D UNIFORM_SAMPLER0;

out vec4 OUT_COLOR;

in vec4 vColor;
in vec2 vCoords;

void main()
{
    OUT_COLOR = vec4((vColor.rgb + texture(UNIFORM_SAMPLER0, vCoords).rgb) * vec3(0.3,0.4,0.45), 1.0);
}

