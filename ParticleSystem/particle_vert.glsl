#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;

in vec2 instancePosition;
in float instanceScale;
in vec4 instanceColor;
in float instanceLife;
in float instanceTime;

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