#version 440
layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 colAttr;

layout(location = 0) uniform mat4 PVM;
out vec3 color;

void main(void) {
    gl_Position = PVM * vec4(posAttr, 1.0);
    color = colAttr;
}
