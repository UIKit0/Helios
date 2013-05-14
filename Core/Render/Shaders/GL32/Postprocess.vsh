#version 150

in vec4 ATTR_POS;
in vec2 ATTR_TEX;

out vec2 vCoords;

void main()
{

    gl_Position = ATTR_POS;
    vCoords = ATTR_TEX;
    
}
