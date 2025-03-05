#version 460
in vec2 uv;
in vec3 vertexOut;

layout(location = 0) out vec4 texCol;

layout(binding=0) uniform sampler2D tex0;
layout(binding=1) uniform sampler2D tex1;
layout(binding=2) uniform sampler2D tex2;
layout(binding=3) uniform sampler2D prevFrame;

void main(){
	vec2 xy = uv - 0.5;

	//texCol = texture(tex0, uv);
	vec4 col = texture(tex2, uv);
	vec4 prevFrameCol = texture(prevFrame, uv);
	if(length(prevFrameCol.xyz) > 0.2)
	{
		texCol = prevFrameCol * vec4(uv, 0., 1.);
	}else
	{
		texCol = col;
	}

}