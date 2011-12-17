
attribute vec4 ATTR_POS;
//attribute vec3 ATTR_NORM;
attribute vec2 ATTR_TEX;
attribute float ATTR_BONEID;

uniform mat4 UNIFORM_MV;
uniform mat4 UNIFORM_P;
//uniform mat3 UNIFORM_NM;
//uniform vec4 UNIFORM_LIGHTPOS;
uniform mat4 UNIFORM_JOINTS[20];
uniform mat3 UNIFORM_TEXMAT;

varying mediump vec4 vColor;
varying mediump vec2 vCoords;
varying mediump vec4 vPos;

void main()
{
    vec4 pos = UNIFORM_MV * UNIFORM_JOINTS[int(ATTR_BONEID)] * ATTR_POS;
    
    //vec4 L = UNIFORM_MV * UNIFORM_LIGHTPOS;
    //vec4 lightDir = pos - L;
    //vColor = ATTR_DIFFUSE;
    vCoords = vec2(UNIFORM_TEXMAT * vec3(ATTR_TEX,1.));
    
    vPos = UNIFORM_P * pos;

    gl_Position = vPos;
    
    vPos.x /= vPos.w;
    vPos.y /= vPos.w;
    vPos.z /= vPos.w;
    
    vPos = vPos * 0.5 + 0.5;

}
