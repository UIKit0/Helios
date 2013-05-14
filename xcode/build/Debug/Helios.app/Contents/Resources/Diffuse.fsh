#version 150

uniform sampler2D UNIFORM_SAMPLER0;

out vec4 OUT_COLOR;
out vec4 OUT_NORMALS;

in vec4 vColor;
in vec2 vCoords;
in vec3 vNormals;

void main()
{
    OUT_COLOR = vec4((vColor.rgb + texture(UNIFORM_SAMPLER0, vCoords).rgb), 1.0);
    OUT_NORMALS = vec4(vNormals * 0.5 + 0.5, 1.0);
}

