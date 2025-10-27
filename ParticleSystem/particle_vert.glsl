#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;

layout(location = 3) in vec2 instancePosition;
layout(location = 4) in float instanceScale;
layout(location = 5) in vec4 instanceColor;
layout(location = 6) in float instanceLife;
layout(location = 7) in float instanceTime;

uniform mat4 mvp;

out vec2 fragTexCoord;
out vec4 fragColor;
out float fragLife;
out float fragTime;

void main()
{
	//Scale
	vec2 scaledPos = vertexPosition.xy * instanceScale;

	//Translate
	vec2 worldPos = scaledPos + instancePosition;
	//Transform
	
	gl_Position = mvp * vec4(worldPos, 0.0, 1.0);

	fragTexCoord = vertexTexCoord;
	fragColor = instanceColor;
	fragLife = instanceLife;
	fragTime = instanceTime;
}