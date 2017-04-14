#version 330

in vec2 UV;
in float color_coef;
in float point_y;

uniform sampler2D textureSampler;

out vec4 outColor;

void main() {
    outColor = texture(textureSampler, UV).rgba;
    if(outColor.a < 1.)
        discard;
    float t = (point_y * 0.4 + 0.6);
    outColor *= color_coef * t*t;
    outColor[3] = 1.;
}
