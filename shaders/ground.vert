#version 330

in vec4 point;
in vec2 uvpoint;

uniform mat4 camera;

out vec2 UV;

void main() {
    UV = uvpoint;
    gl_Position = camera * point;
}
