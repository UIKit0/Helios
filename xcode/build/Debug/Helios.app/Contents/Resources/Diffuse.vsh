#version 150

in vec4 ATTR_POS;
in vec3 ATTR_NORM;
in vec4 ATTR_DIFFUSE;
in vec2 ATTR_TEX;
in float ATTR_BONEID;

uniform mat4 UNIFORM_MV;
uniform mat4 UNIFORM_P;
uniform mat3 UNIFORM_NM;
uniform vec4 UNIFORM_LIGHTPOS;
uniform mat4 UNIFORM_JOINTS[50];

out vec4 vColor;
out vec2 vCoords;
out vec3 vNormals;

void main()
{

    mat4 j = UNIFORM_JOINTS[int(ATTR_BONEID)];
    mat4 m = UNIFORM_MV * j;

    gl_Position = UNIFORM_P * m * ATTR_POS;

    vNormals = normalize(UNIFORM_NM * (ATTR_NORM*2.0-1.0));
    vColor = ATTR_DIFFUSE  ;
    vCoords = ATTR_TEX;
}
