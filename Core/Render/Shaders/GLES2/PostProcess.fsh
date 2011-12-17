precision mediump float;

uniform sampler2D UNIFORM_SAMPLER0;

varying vec2 vCoord;

void main()
{

    vec4 c = texture2D(UNIFORM_SAMPLER0, vCoord);
    
    gl_FragColor = c;

}
