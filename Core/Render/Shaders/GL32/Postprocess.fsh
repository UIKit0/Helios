#version 150
#define MAX_LIGHTS 10

uniform sampler2D UNIFORM_SAMPLER0; // diffuse color
uniform sampler2D UNIFORM_SAMPLER1; // normal buffer
uniform sampler2D UNIFORM_SAMPLER2; // shadow buffer
uniform sampler2D UNIFORM_SAMPLER3; // depth buffer

uniform vec4 UNIFORM_LIGHT_COLOR[MAX_LIGHTS];
uniform vec4 UNIFORM_LIGHT_POS[MAX_LIGHTS];
uniform int  UNIFORM_LIGHT_COUNT;

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
    vec3 pos = vec3(vCoords.xy * 2.0 - 1.0, depth * 2.0 - 1.0);
    vec3 norm = texture(UNIFORM_SAMPLER1, coords).rgb * 2.0 - 1.0;
    
    int i ;
    vec4 lightcolor = vec4(0.0,0.0,0.0,0.0);
    for ( i = 0 ; i < UNIFORM_LIGHT_COUNT ; i ++ )
    {
        vec3 light = vec3(UNIFORM_LIGHT_POS[i].xyz / UNIFORM_LIGHT_POS[i].w);
        vec3 lightDir = pos - light;
        lightcolor += UNIFORM_LIGHT_COLOR[i] * max(0.0, dot(lightDir, norm));
    }
    vec4 shadow = texture(UNIFORM_SAMPLER2, coords);
    vec4 shaded = vec4(texture(UNIFORM_SAMPLER0, coords).rgb * shadow.rgb * lightcolor.rgb , 1.0);
    
    OUT_COLOR = shaded;

}

