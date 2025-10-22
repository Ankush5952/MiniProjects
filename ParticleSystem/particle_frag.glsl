#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

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
		float x = (timeSinceLifeBegan/lifetime);
		float fadeAmount = 1.0 - pow(x,5);
		finalColor = vec4(fragColor.rgb, fragColor.a * fadeAmount);
	}

	//glow
	if(glowEnabled == 1)
	{
		float distFromCenter = length(fragTexCoord - vec2(0.5,0.5));
		clamp(distFromCenter, 0 , 1);
		float t = (1.0 - distFromCenter);
		t = pow(t, 2.0);
		finalColor.rgb += fragColor.rgb * t * glowIntensity;
		//finalColor = vec4(distFromCenter, 0, 0, fragColor.a);
	}
}