#version 140
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

in vec2 textureCoords;
in vec4 ambientLightPass;

in vec3 normalDirectionCS;
in vec3 lightDirectionCS;

out vec4 pixel;

const int pcfCount = 2;
const float texelCount = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
const float mapSize = 2048 * 2;
const float texelSize = 1.0 / mapSize;

layout(location = 0) uniform sampler2D albedo;
layout(location = 1) uniform sampler2D normal;
layout(location = 2) uniform sampler2D lighting;
layout(location = 3) uniform sampler2D glow;
layout(location = 4) uniform sampler2D position;
layout(location = 5) uniform sampler2D shadowMap;

//layout(location = 10) uniform sampler2D settings;

uniform vec3 sunDirection;
uniform vec3 cameraPosition;
uniform float farPlane;
uniform mat4 viewMatrix;
uniform mat4 depthMVP;
uniform float rain;
uniform vec4 ambientLight;

/*vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);*/

vec2 poissonDisk[4] = vec2[](
  vec2(0, 0),
  vec2(0, 0),
  vec2(0, 0),
  vec2(0, 0)
);

struct PointLight
{
	vec3 position;
	vec3 colour;
	float length;
	float falloff;
	float power;
};

uniform PointLight pointLights[100];
uniform float pointLightCount;

float no = 0.1;

float LIGHT_COUNT = 1;

float random(vec4 seed)
{
	float dotted = dot(seed, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dotted) * 43758.5453);
}
float sunLighting(vec4 surfaceNormal)
{
    return dot(normalize(surfaceNormal.xyz), sunDirection);
}

float calculateShadow(vec4 shadowCoord)
{
	if(shadowCoord.x < 0 || shadowCoord.y < 0 || shadowCoord.x > 1 || shadowCoord.y > 1 || texture(shadowMap, vec2(shadowCoord.x, shadowCoord.y)).r >= 1)
	{
		return 1.0;
	}
	float total = 0.0;
	for(int x = -pcfCount; x < pcfCount; x++)
	{
		for(int y = -pcfCount; y < pcfCount; y++)
		{
			float surface = texture(shadowMap, vec2(shadowCoord.x, shadowCoord.y) + vec2(x, y) * texelSize).r;
			if (surface + 0.002 < shadowCoord.z)
			{
				for (int i = 0; i < 4; i++)
				{	 
					int index = int(16.0 * random(vec4(gl_FragCoord.xyy, i)))%16;
					float r = texture(shadowMap, vec2(shadowCoord.xy + poissonDisk[index]/700.0)).r;
					if(r >= 0.999)
					{
						r = 1;
					}
					else
					{
						r = 0;
					}
					total += 0.1*(1.0-r);
					//total += 1.0;
				}
			}
		}
	}
	return total /= texelCount;
}
float pMultiplier = 1;
vec4 calculateLight(vec3 position, vec3 normal)
{
	pMultiplier = 1;
	vec4 outc = vec4(0, 0, 0, 0);
	float total;
	for(int i = 0; i < pointLightCount; i++)
	{
		vec3 pLightPosition = pointLights[i].position;
		vec3 pLightColour = pointLights[i].colour;
		float pLightLength = pointLights[i].length;
		float pLightPower = pointLights[i].power;
		pMultiplier = max(pMultiplier, pLightPower);
		float pLightFalloff = pointLights[i].falloff;
		float distance = abs(distance(pLightPosition, position));
		float lightPower = 0;
		if(distance < pLightLength)
		{
			float dist = (pLightLength - distance) * 3;
			lightPower = 1.0 / (1.0 + 0.1*dist + 0.01*dist*dist);//(pLightLength - distance) / pLightLength;
			lightPower = 1 - lightPower;
		}
	
		float nDot = dot(normalize(normal.xyz), normalize(pLightPosition - position));
		float brightness = nDot;
		lightPower *= (brightness + 1) / 2;
		if(nDot < 0.0)
		{
			lightPower = 0;
		}
		lightPower /= pLightFalloff;
		vec3 lc = pLightColour;
		if(distance < pLightLength)
		{
			outc.w += lightPower * pLightPower;
			outc.xyz += lc * lightPower * pLightPower;
		}
	}
	if(pointLightCount == 0)
	{
		outc = vec4(1, 1, 1, 1);
	}
	outc.xyz;
	return outc;
}

void main()
{
	vec4 normalColour = texture(normal, textureCoords);
	vec4 unnormalized = ((vec4(normalColour.xyz, 1.0) * 2) - 1);
    vec4 positionColour = texture(position, textureCoords);
	vec4 lightingColour = texture(lighting, textureCoords);
	vec4 albedoColour = texture2D(albedo, vec2(textureCoords.x, textureCoords.y));

	float reflectivity = lightingColour.x * 0;
	float shineDamper = 0.0;//lightingColour.y * 100;

	float lightingType = lightingColour.z;

	pixel = albedoColour;
	pixel.w = 0;
	if((normalColour.x != 1 || normalColour.y != 1 || normalColour.z != 1) && !(unnormalized.x == 1 && unnormalized.y == 1 && unnormalized.z == 1))
	{
		vec4 light = ambientLight;
		vec3 cameraLocation = (positionColour.xyz * farPlane * 2 - farPlane + cameraPosition);
		cameraLocation = vec3(((positionColour.xyz * 2) - 1) * farPlane) + cameraPosition;
		vec4 realPosition = vec4(cameraLocation.x + unnormalized.x * no, cameraLocation.y + unnormalized.y * no, cameraLocation.z + unnormalized.z * no, 1);
		float distanceToCam = sqrt((pow(realPosition.x - cameraPosition.x, 2) + pow(realPosition.z - cameraPosition.z, 2)));
		vec4 shadowCoord = depthMVP * realPosition;
		unnormalized = -unnormalized;
		float total = calculateShadow(shadowCoord);

		vec4 sunColour = vec4(0, 0, 0, 1);
		float dottedNormal = 1 - sunLighting(unnormalized);
		float sunBrightness = ((1.5) * (dottedNormal + 1) / 2) - 0.5;
		
		float nDot = dot(normalize(unnormalized.xyz), (sunDirection));
		float brightness = (nDot + 1) / 2;
		brightness += 0.2;
		brightness /= 1.2;

		float surface = texture(shadowMap, vec2(shadowCoord.x, shadowCoord.y)).r;
		
		float visibility = 1;

		float shadow = (total);
		
		vec3 toCameraVector = (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - cameraLocation;
		vec3 incidenceVector = sunDirection; //a unit vector
		vec3 reflectionVector = reflect(incidenceVector, unnormalized.xyz);
		vec3 surfaceToCamera = normalize(toCameraVector); //also a unit vector
		float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
		float dampedFactor = pow(cosAngle, shineDamper);
		if(brightness >= 0.5 && visibility < 1)
		{	
			reflectivity *= (visibility - 0.75) * 4;
			if(reflectivity < 0)
			{	
				reflectivity = 0;
			}
		}
		float spec = dampedFactor * reflectivity;
		pixel = albedoColour;
		brightness = (nDot + 1) / 2;
		brightness = (brightness + 1)/2;

		//float mi = 0.2;

		//brightness += mi;
		//brightness /= (1 + mi);

		//brightness = -pow(0.9 - brightness * 0.9, 2) + 1;
		//brightness -= 0.5;
		//brightness *= 2;
		
		//brightness *= 1 + spec;

		if(lightingType >= 1)
		{
			if(brightness < 0.1)
			{
				brightness = 0.8;
			}
			else if(brightness < 0.6)
			{
				brightness = 1.2;
			}
			else if(brightness < 0.9)
			{
				brightness = 1.6;
			}
			else
			{
				brightness = 2;
			}
		}
		else
		{
			if(nDot >= 0.0)
			{
				if(shadow > 0.0 && shadow < 1.0)
				{
					brightness -= (shadow * 1) * nDot;
					spec = 0;
				}
			}
		}

		pixel.xyz *= brightness;
		float viewDistance = farPlane;
		float interpolated = sqrt(1 - pow(distanceToCam / viewDistance, 2));
		if(distanceToCam > viewDistance)
		{
			interpolated = 0;
		}
		pixel = mix(vec4((ambientLight.r + 2) / 3, (ambientLight.r + 2) / 3, (ambientLight.r + 2) / 3, 1), pixel, interpolated);
		pixel *= ambientLight.r;
	}
	pixel.w = 1;
}