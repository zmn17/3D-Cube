#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 aColor;

out vec3 color;

// uniform float scale;
uniform mat4 mvMat;
uniform mat4 projMat;

void main(void){
	gl_Position = projMat * mvMat * vec4(pos, 1.0f); 
	color = aColor;
}
