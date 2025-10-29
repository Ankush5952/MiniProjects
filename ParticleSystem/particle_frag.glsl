#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

in float fragLife;
in float fragTime;


out vec4 finalColor;


uniform int fadeEnabled;

uniform int glowEnabled;
uniform float glowIntensity;

void main()
{
	finalColor = fragColor;


	//fade
	if(fadeEnabled == 1)
	{
		float x = (fragTime/fragLife);
		float fadeAmount = 1.0 - pow(x,5.0);
		finalColor = vec4(fragColor.rgb, fragColor.a * fadeAmount);
	}

	//glow	
	if(glowEnabled == 1)
	{
		float distFromCenter = length(fragTexCoord - vec2(0.5,0.5))/0.7071;
		distFromCenter = clamp(distFromCenter, 0.0, 1.0);
		float t = ( distFromCenter);
		t = pow(t, 5.0);
		
		finalColor = vec4(finalColor.rgb + finalColor.rgb * t * glowIntensity, finalColor.a);
	}
}