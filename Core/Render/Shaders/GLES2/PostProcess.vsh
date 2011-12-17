
attribute vec4 ATTR_POS;

varying vec2 vCoord;

void main()
{
    vCoord = pos.xy * 0.5 + 0.5;
    gl_Position = pos;
}
