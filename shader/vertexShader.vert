#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 v_vertexColors;
out vec2 TexCoord;

uniform mat4 transform;

void main() {
  v_vertexColors = aColor;
  TexCoord = aTexCoord;
  gl_Position = transform * vec4(aPos, 1.0);
}