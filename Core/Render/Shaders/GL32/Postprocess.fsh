#version 150

uniform sampler2D UNIFORM_SAMPLER0; // diffuse color
uniform sampler2D UNIFORM_SAMPLER1; // normal buffer
uniform sampler2D UNIFORM_SAMPLER2; // shadow buffer
uniform sampler2D UNIFORM_SAMPLER3; // depth buffer

out vec4 OUT_COLOR;

in vec2 vCoords;

void main()
{
    vec2 coords = vec2(vCoords.x, 1.0 - vCoords.y);
    // combine geometry and shadow buffers
    vec4 shaded = vec4(texture(UNIFORM_SAMPLER0, coords).rgb * (texture(UNIFORM_SAMPLER2, coords).rgb) , 1.0);
    
    OUT_COLOR = shaded;

}

