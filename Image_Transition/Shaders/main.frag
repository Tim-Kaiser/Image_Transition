#version 460
in vec2 vUv;
in vec3 vertexOut;

uniform sampler2D texSimOut;

out vec4 fragCol;

void main(){
	vec4 col = texture(texSimOut, vUv);

	fragCol = col;
}