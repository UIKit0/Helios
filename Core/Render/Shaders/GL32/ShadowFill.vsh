#version 150

in vec4 ATTR_POS;
in vec3 ATTR_NORM;
in vec4 ATTR_DIFFUSE;
in vec2 ATTR_TEX;
in float ATTR_BONEID;

uniform mat4 UNIFORM_MV;
uniform mat4 UNIFORM_P;
uniform mat4 UNIFORM_JOINTS[50];

out vec4 vColor;
out vec2 vCoords;

void main()
{
    
    mat4 j = UNIFORM_JOINTS[int(ATTR_BONEID)];
    mat4 m = UNIFORM_MV * j;
    
    gl_Position = UNIFORM_P * m * ATTR_POS;
    vCoords = ATTR_TEX;
    vColor = ATTR_DIFFUSE;
    
}
