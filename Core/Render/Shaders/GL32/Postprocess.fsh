#version 150

uniform sampler2D UNIFORM_SAMPLER0; // diffuse color
uniform sampler2D UNIFORM_SAMPLER1; // normal buffer
uniform sampler2D UNIFORM_SAMPLER2; // shadow buffer
uniform sampler2D UNIFORM_SAMPLER3; // depth buffer

uniform vec2 UNIFORM_FBOSIZE;

out vec4 OUT_COLOR;

in vec2 vCoords;

vec4 multisample(sampler2D sampler, vec2 coord)
{
    float tex_x = 1.0 / UNIFORM_FBOSIZE.x * 0.66666;
    float tex_y = 1.0 / UNIFORM_FBOSIZE.y * 0.66666;
    
    vec4 sample;
    sample = texture(sampler, coord);


    return sample;
}
void main()
{
    vec2 coords = vec2(vCoords.x, 1.0 - vCoords.y);
    // combine geometry and shadow buffers
    
    float depth = texture(UNIFORM_SAMPLER3, coords).r;
    vec4 shadow = texture(UNIFORM_SAMPLER2, coords);
    vec4 shaded = vec4(texture(UNIFORM_SAMPLER0, coords).rgb * shadow.rgb , 1.0);
    
    OUT_COLOR = shaded;

}

