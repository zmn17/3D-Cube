#version 460

in vec3 outColor;
in vec2 texCoords;

out vec4 FragColor;

layout (binding = 0) uniform sampler2D samp;
uniform bool useTexture;

void main(void){
	if(useTexture){
		vec4 texColor = texture(samp, texCoords);
		FragColor = texColor * vec4(outColor, 1.0);
	}else{
		FragColor =  vec4(1.0,0.0,0.0,1.0);
	}
}

