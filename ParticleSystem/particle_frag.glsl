#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform float lifetime;
uniform float timeSinceLifeBegan;
uniform int fadeEnabled;

void main()
{
	//fade
	if(fadeEnabled == 1)
	{
		float fadeAmount = 1.0 - (timeSinceLifeBegan/lifetime);
		finalColor = vec4(fragColor.rgb, fragColor.a * fadeAmount);
	}else
	{
		finalColor = fragColor;
	}
}