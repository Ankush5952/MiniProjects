#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;

layout(location = 10) in vec2 instancePosition;
layout(location = 11) in float instanceScale;
layout(location = 12) in vec4 instanceColor;
layout(location = 13) in float instanceLife;
layout(location = 14) in float instanceTime;

uniform mat4 mvpMat;

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
	gl_Position = mvpMat * vec4(worldPos, 0.0, 1.0);

	fragTexCoord = vertexTexCoord;
	fragColor = instanceColor;
	fragLife = instanceLife;
	fragTime = instanceTime;
}