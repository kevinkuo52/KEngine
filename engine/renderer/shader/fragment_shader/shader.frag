#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

vec3 mainLightPos = vec3(10.0f, 10.0f, 10.0f);

void RayGen(){
    
}

void main() {
    outColor = vec4(0.1f * fragColor * texture(texSampler, fragTexCoord).rgb, 1.0);
}