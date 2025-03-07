#version 460
in vec2 vUv;
in vec3 vertexOut;

uniform sampler2D texSimIn;
uniform sampler2D startFrame;
uniform sampler2D endFrame;

uniform int setup;
uniform float factor;
uniform float time;

layout (location = 0) out vec4 texSimOut;

out vec4 fragCol;

#define color_dark vec4(25.0, 54.0,52.0 ,255.0)
#define color_light vec4(159.0, 201.0, 196.0,255.0)
#define color_grey vec4(69.0, 92.0, 84.0,255.0)



float snoise(in vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453)-.1;
}



float getNeighborValue()
{
	vec2 texCoord =  gl_FragCoord.xy;

	float neighborAliveCount = 0.;

	neighborAliveCount += float( texelFetch(texSimIn, ivec2(texCoord) + ivec2(1.,0.), 0).r > 0.);
	neighborAliveCount += float( texelFetch(texSimIn, ivec2(texCoord) + ivec2(-1.,0.), 0).r > 0.);
	neighborAliveCount += float( texelFetch(texSimIn, ivec2(texCoord) + ivec2(0.,1.), 0).r > 0.);
	neighborAliveCount += float( texelFetch(texSimIn, ivec2(texCoord) + ivec2(0.,-1.), 0).r > 0.);

	neighborAliveCount += float( texelFetch(texSimIn, ivec2(texCoord) + ivec2(1.,1.), 0).r > 0.);
	neighborAliveCount += float( texelFetch(texSimIn, ivec2(texCoord) + ivec2(1.,-1.), 0).r > 0.);
	neighborAliveCount += float( texelFetch(texSimIn, ivec2(texCoord) + ivec2(-1.,1.), 0).r > 0.);
	neighborAliveCount += float( texelFetch(texSimIn, ivec2(texCoord) + ivec2(-1.,-1.), 0).r > 0.);
	
	return neighborAliveCount;
}

void main(){

	vec4 prevFrameCol = texture(texSimIn, vUv);
	vec4 goalTexCol = texture(endFrame, vUv);
	vec4 startTexCol = texture(startFrame, vUv);

	if(setup == 1)
	{
		vec4 noise = vec4(snoise(vUv) > 0.8 ? 1.0 : 0.0);
		texSimOut = vec4(startTexCol.x > 0.1 ? 1.0 : 0.0);
	}else{
		float finalColor = prevFrameCol.x;

		float neighborAliveCount = getNeighborValue();

		// resurect if we are not live, and have 3 live neighrbours
        finalColor += (1.0-float(prevFrameCol.x > 0.0)) * float(neighborAliveCount == 3.0);

        // kill if we do not have either 3 or 2 neighbours
        finalColor *= min(float(neighborAliveCount == 2.0) + float(neighborAliveCount == 3.0) + float(neighborAliveCount == 6.0), 1.);

		float isNotPartOfGoalTex = float(goalTexCol.x < 0.1);
		float isPartOfGoalTex = float(goalTexCol.x > 0.3);
		float isEnd = float(factor > 0.1);

		finalColor -= isNotPartOfGoalTex * factor;

		
		finalColor += isEnd * (isPartOfGoalTex * 0.2);

		finalColor = clamp(finalColor, 0., 1.);

		texSimOut = vec4(vec3(finalColor), 1.);
	}
}