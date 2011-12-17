varying mediump vec4 vColor;
varying mediump vec2 vCoords;
uniform sampler2D UNIFORM_SAMPLER0;
void main()

{
    gl_FragColor = vec4((vColor.rgb + texture2D(UNIFORM_SAMPLER0, vCoords).rgb) * 0.5, 1.0);
}
