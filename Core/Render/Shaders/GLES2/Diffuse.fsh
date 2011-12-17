varying lowp vec4 colorVarying;
varying mediump vec2 vCoords;

varying mediump vec4 vColor;

uniform mediump vec3 UNIFORM_LIGHTPOS;
uniform sampler2D UNIFORM_SAMPLER0;

void main()
{
    gl_FragColor = vec4(1.,1.,1.,1.);// vec4((vColor.rgb + texture2D(UNIFORM_SAMPLER0, vCoords).rgb) * 0.3, 1.0);
}
