#version 460
in vec2 uv;
in vec3 vertexOut;
layout(location = 0) out vec4 fragCol;

uniform sampler2D tex;

void main(){
	vec2 xy = uv - 0.5;



	// fragCol = vec4(xy,0.,1.);

	fragCol = texture(tex, uv);
}