#version 150

in vec4 ATTR_POS;
in vec3 ATTR_NORM;
in vec4 ATTR_DIFFUSE;
in vec2 ATTR_TEX;
in float ATTR_BONEID;
in float ATTR_NO_EXTRUDE;
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
    mat4 j = UNIFORM_JOINTS[int(ATTR_BONEID)];
    mat4 m = UNIFORM_MV * j;
    
    vec4 pos = m * ATTR_POS;
   
    mat3 n = mat3( transpose( inverse( m ) ) );
    vec4 L = UNIFORM_MV * UNIFORM_LIGHTPOS;
    vec4 lightDir = pos - L;
    vColor = ATTR_DIFFUSE;
    vCoords = ATTR_TEX;
    
    
    if ( dot (lightDir.xyz, n * (ATTR_NORM*2.0-1.0))<0.  ) {
        // Move the shadow caster slightly further away from the light source so flat surfaces don't
        // get artifacts from the depth test conflicting
        gl_Position = UNIFORM_P * (pos + (normalize(lightDir)*0.1));
    } else {
        gl_Position = UNIFORM_P * vec4(pos.xyz * L.w - L.xyz,0.);
    }
    
    
}
