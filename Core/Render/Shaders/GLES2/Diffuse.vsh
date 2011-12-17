

varying lowp vec4 colorVarying;
varying mediump vec2 vCoords;
varying mediump vec3 vEyeNormal;
varying mediump vec4 vColor;

attribute vec4 ATTR_POS;
attribute vec3 ATTR_NORM;
attribute vec4 ATTR_DIFFUSE;
attribute vec2 ATTR_TEX;
attribute float ATTR_BONEID;
attribute float ATTR_NO_EXTRUDE;

uniform mat4 UNIFORM_MV;
uniform mat4 UNIFORM_P;
uniform mat3 UNIFORM_NM;

uniform mat4 UNIFORM_JOINTS[20];

void main()
{    
    
    vColor = ATTR_DIFFUSE  ;
    gl_Position = UNIFORM_P * UNIFORM_MV  * UNIFORM_JOINTS[int(ATTR_BONEID)] * ATTR_POS;
    vCoords = ATTR_TEX;
}
