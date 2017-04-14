#version 330

in vec2 UV;

uniform sampler2D textureSampler;

out vec4 outColor;

void main() {
    vec3 color = texture(textureSampler, UV).rgb;
    float threshold = .7;
    if(color.r > threshold && color.g > threshold && color.b > threshold){
        discard;
    }

    outColor = vec4(color, 1.);
}
