#version 150

uniform sampler2D UNIFORM_SAMPLER0;

out vec4 OUT_COLOR;

in vec4 vColor;
in vec2 vCoords;

void main()
{
    // TODO: Output a solid shadow color that can be combined in post-process.  We do not need to sample textures here.
    OUT_COLOR =  vec4(0.4,0.5,0.6,1.0);
}

