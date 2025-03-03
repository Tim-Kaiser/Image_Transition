#version 460
in vec2 uv;
in vec3 vertexOut;
layout(location = 0) out vec4 fragCol;

layout(binding=0) uniform sampler2D tex0;
layout(binding=1) uniform sampler2D tex1;
layout(binding=2) uniform sampler2D tex2;

void main(){
	vec2 xy = uv - 0.5;

	// fragCol = vec4(xy,0.,1.);

	fragCol = texture(tex2, uv);
}