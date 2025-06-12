#version 460

in vec3 color;
in vec2 texCoords;

out vec4 FragColor;

layout (binding = 0) uniform sampler2D samp;

void main(void){
	FragColor = texture(samp, texCoords);
}

