
attribute vec4 ATTR_POS;
attribute vec3 ATTR_NORM;
attribute vec4 ATTR_DIFFUSE;
attribute vec2 ATTR_TEX;
attribute float ATTR_BONEID;
attribute float ATTR_NO_EXTRUDE;

uniform mat4 UNIFORM_MV;
uniform mat4 UNIFORM_P;
uniform mat3 UNIFORM_NM;
uniform vec4 UNIFORM_LIGHTPOS;
uniform mat4 UNIFORM_JOINTS[20];

varying mediump vec4 vColor;
varying mediump vec2 vCoords;
void main()
{
    vec4 pos = UNIFORM_MV * UNIFORM_JOINTS[int(ATTR_BONEID)] * ATTR_POS;
    
    vec4 L = UNIFORM_MV * UNIFORM_LIGHTPOS;
    vec4 lightDir = pos - L;
    vColor = ATTR_DIFFUSE;
    vCoords = ATTR_TEX;
    
    
    if ( dot (lightDir.xyz, UNIFORM_NM * ATTR_NORM)<0. || ATTR_NO_EXTRUDE > 0.) {
        // Move the shadow caster slightly further away from the light source so flat surfaces don't
        // get artifacts from the depth test conflicting
        gl_Position = UNIFORM_P * (pos + (normalize(lightDir)*0.1));
    } else {
        gl_Position = UNIFORM_P * vec4(pos.xyz * L.w - L.xyz,0.);
    }
    
   

}
