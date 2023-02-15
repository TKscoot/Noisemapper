#version 330 core

// vertex shader

// input:  attribute named 'position' with 3 floats per vertex
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;
layout (location = 2) in vec2 texCoords;

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;
out vec4 fragColor;
out vec3 fragPos;

void main() {
  fragPos = vec3(world * vec4(position, 1.0));

  fragColor = vec4(uv, 1.0);

  gl_Position = proj * view * world * vec4(position, 1.0);
}
