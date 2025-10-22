#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
in float distFromCenter;

out vec4 finalColor;

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
		if(distFromCenter > 0.45)
		{
			float t = (1.0 - distFromCenter);
			t = pow(t, 2.0);
			finalColor.rgb += fragColor.rgb * t * glowIntensity;
		}
	}
}