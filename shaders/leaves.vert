#version 330

in vec4 point;
in vec2 uvpoint;

uniform vec3 position;
uniform float scale;
uniform mat4 camera;

out vec2 UV;

void main() {
    mat4 translationMatrix = mat4(1.);
    translationMatrix[3][0] = position.x;
    translationMatrix[3][1] = position.y;
    translationMatrix[3][2] = position.z;


    mat4 scaleMatrix = mat4(1.);
    scaleMatrix[0][0] = scale;
    scaleMatrix[1][1] = scale;
    scaleMatrix[2][2] = scale;



    UV = uvpoint;
    gl_Position = camera * translationMatrix * scaleMatrix * point;
}
