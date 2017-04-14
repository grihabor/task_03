#version 330

in vec2 UV;
in float y;

uniform sampler2D textureSampler;

out vec4 outColor;

void main() {
    vec3 color = texture(textureSampler, UV).rgb * 1.2 * (y - .3);
    outColor = vec4(color, 1.);
}
