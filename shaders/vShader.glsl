#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 tex;

out vec3 outColor;
out vec2 texCoords;

uniform float scale;
uniform mat4 mvMat;
uniform mat4 projMat;

void main(void){
	gl_Position = projMat * mvMat * vec4(pos * scale, 1.0f); 
	outColor = aColor;
	texCoords = tex;
}
