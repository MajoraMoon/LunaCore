#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 v_vertexColors;
out vec2 TexCoord;

void main()
{
    v_vertexColors = aColor;
    TexCoord = aTexCoord;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}