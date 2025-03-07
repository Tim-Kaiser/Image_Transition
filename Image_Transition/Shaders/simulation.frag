#version 460
in vec2 vUv;
in vec3 vertexOut;

uniform sampler2D texSimIn;
uniform sampler2D startFrame;
uniform sampler2D endFrame;

uniform int setup;


layout (location = 0) out vec4 texSimOut;

out vec4 fragCol;


float getNeighborValue()
{
	vec2 texCoord =  gl_FragCoord.xy;

	float neighborAliveCount = 0.;

	neighborAliveCount += texelFetch(texSimIn, ivec2(texCoord) + ivec2(1.,0.), 0).r;
	neighborAliveCount += texelFetch(texSimIn, ivec2(texCoord) + ivec2(-1.,0.), 0).r;
	neighborAliveCount += texelFetch(texSimIn, ivec2(texCoord) + ivec2(0.,1.), 0).r;
	neighborAliveCount += texelFetch(texSimIn, ivec2(texCoord) + ivec2(0.,-1.), 0).r;

	neighborAliveCount += texelFetch(texSimIn, ivec2(texCoord) + ivec2(1.,1.), 0).r;
	neighborAliveCount += texelFetch(texSimIn, ivec2(texCoord) + ivec2(1.,-1.), 0).r;
	neighborAliveCount += texelFetch(texSimIn, ivec2(texCoord) + ivec2(-1.,1.), 0).r;
	neighborAliveCount += texelFetch(texSimIn, ivec2(texCoord) + ivec2(-1.,-1.), 0).r;
	
	return neighborAliveCount;
}

bool isAlive()
{
	bool currentlyAlive = false;
	if(texture(texSimIn, vUv).x > 0.9)
	{
		currentlyAlive = true;
	}

	float neighborAliveCount = getNeighborValue();

	if(currentlyAlive)
	{
		if((neighborAliveCount > 1.9 && neighborAliveCount < 2.2))
			{
				return true;
			}
	}else
	{
		if((neighborAliveCount > 1.9 && neighborAliveCount < 2.5))
		{
			return true;
		}
	}
	return false;

#if 0
	if(currentlyAlive)
	{
		if(neighborAliveCount < 2.0)
		{
			return false;
		}
		else if((neighborAliveCount > 2.5 && neighborAliveCount < 3.5) || (neighborAliveCount > 5.5 && neighborAliveCount < 6.5))
		{
			return true;
		}
		else{
			return false;
		}
	
	}else{
		if((neighborAliveCount > 2.5 && neighborAliveCount < 3.5) || (neighborAliveCount > 5.5 && neighborAliveCount < 6.5))
		{
			return true;
		}
		else{
			return false;
		}
	}
#endif
#if 0
	if(neighborAliveCount < 2.0)
	{
		return false;
	}
	if(neighborAliveCount == 3)
	{
		return true;
	}
	if(neighborAliveCount == 2 && currentlyAlive)
	{
		return true;
	}
	if(neighborAliveCount == 6 && !currentlyAlive)
	{
		return true;
	}
	return false;
#endif
}

vec3 getDecayColor()
{
	vec3 decayed = (texture(texSimIn, vUv).xyz) * vec3(0.5);

	if(decayed.x < 0.1)
	{
		decayed = vec3(0.);
	}
	return decayed;
}


void main(){

	vec4 prevFrameCol = texture(texSimIn, vUv);
	vec4 goalTexCol = texture(endFrame, vUv);
	vec4 startTexCol = texture(startFrame, vUv);

	if(setup == 1)
	{
		texSimOut = vec4(startTexCol.xyz, 1.);
	}else{
		vec4 finalColor = vec4(0.);

		if(isAlive())
		{
			finalColor = vec4(1.);
		}else
		{
			finalColor = vec4(getDecayColor(), 1.);
		}


		//if(prevFrameCol.x > 0.9 && goalTexCol.x > 0.9)
		//{
		//	finalColor = vec4(1.);
		//}
		//else if(isAlive())
		//{
		//	finalColor = vec4(1., 1., 1., 1.);
		//}else
		//{
			//finalColor = vec4(getDecayColor(), 1.);
		//}
		//finalColor = vec4(1., 0.,0., 1.);
		vec4 addCol = vec4(0.001, 0.,0.,1.) + texture(endFrame, vUv) * 0.001;
		vec4 col = finalColor + addCol; 


		texSimOut = finalColor;
	}
}