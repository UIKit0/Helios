#version 150

in vec4 ATTR_POS;
in vec3 ATTR_NORM;
in vec4 ATTR_DIFFUSE;
in vec2 ATTR_TEX;
in float ATTR_BONEID;
in float ATTR_NO_BONES;

uniform mat4 UNIFORM_MV;
uniform mat4 UNIFORM_P;
uniform mat3 UNIFORM_NM;
uniform vec4 UNIFORM_LIGHTPOS;
uniform mat4 UNIFORM_JOINTS[50];

out vec4 vColor;
out vec2 vCoords;

void main()
{
/*
    mat4 j = (ATTR_NO_BONES < 1. ? mat4(1.) : UNIFORM_JOINTS[int(ATTR_BONEID)]);
    mat4 m = UNIFORM_MV * j;
    
    vec4 pos = UNIFORM_P * m * ATTR_POS;
    vec4 L = UNIFORM_MV * UNIFORM_LIGHTPOS;
    //vec4 lightDir = pos - L;
    //float fac = dot(normalize(lightDir.xyz),normalize(UNIFORM_NM * (ATTR_NORM*2.0-1.0))) * 0.5 + 0.5 ;
    vColor = ATTR_DIFFUSE;// * fac  ;
    gl_Position = pos;
    vCoords = ATTR_TEX; */

    mat4 j = ATTR_NO_BONES < 1. ? mat4(1.) : UNIFORM_JOINTS[int(ATTR_BONEID)];
    mat4 m = UNIFORM_MV * j;
    
    vec4 L = UNIFORM_LIGHTPOS;
    
    gl_Position = UNIFORM_P * m * ATTR_POS;
    //vec4 lightDir = gl_Position - L;
    //float fac = dot(normalize(lightDir.xyz),normalize(UNIFORM_NM * (ATTR_NORM*2.0-1.0))) * 0.5 + 0.5 ;
    vColor = ATTR_DIFFUSE ;//* fac  ;
    vCoords = ATTR_TEX;
}
