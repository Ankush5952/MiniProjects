#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;

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
	vec3 scaledPos = instancePosition * instanceScale;

	//Translate
	vec3 worldPos = vec3(instancePosition.x + scaledPos.x,
						 instancePosition.y + scaledPos.y,
						 scaledPos.z);
	//Transform
	gl_Position = mvp * vec4(vertexPosition, 1.0);

	fragTexCoord = vertexTexCoord;
	fragColor = instanceColor;
	fragLife = instanceLife;
	fragTime = instanceTime;
}