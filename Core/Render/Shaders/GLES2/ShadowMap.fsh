precision mediump float;

varying mediump vec4 vColor;
varying mediump vec2 vCoords;
varying mediump vec4 vPos;

uniform sampler2D UNIFORM_SAMPLER0;
void main()

{
    vec4 c = texture2D(UNIFORM_SAMPLER0, vCoords);
    gl_FragColor = vec4(vPos.zzz, c.a);
}
