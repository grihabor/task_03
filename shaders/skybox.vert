#version 330

in vec3 point;
out vec3 TexCoords;

uniform mat4 camera;
uniform vec3 camera_pos;

void main()
{
    gl_Position = camera * vec4(point + camera_pos, 1.0);
    TexCoords = point;
}