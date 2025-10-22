#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
in float sqdistFromCenter;

out vec4 finalColor;

uniform float radius;

uniform float lifetime;
uniform float timeSinceLifeBegan;

uniform int fadeEnabled;

uniform int glowEnabled;
uniform float glowIntensity;

void main()
{
	finalColor = fragColor;

	//fade
	if(fadeEnabled == 1)
	{
		float fadeAmount = 1.0 - (timeSinceLifeBegan/lifetime);
		finalColor = vec4(fragColor.rgb, fragColor.a * fadeAmount);
	}

	//glow
	if(glowEnabled == 1)
	{
		float t = sqdistFromCenter/(radius*radius);
		if(t > glowIntensity*glowIntensity)
		{
			finalColor = vec4(t,t,t, fragColor.a);
		}
	}
}