#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;

uniform mat4 mvp;

uniform vec2 center;

out vec2 fragTexCoord;
out vec4 fragColor;
out float sqdistFromCenter;

void main()
{
	fragTexCoord = vertexTexCoord;
	fragColor = vertexColor;
	gl_Position = mvp * vec4(vertexPosition, 1.0);

	float dx = vertexPosition.x - center.x;
	float dy = vertexPosition.y - center.y;

	sqdistFromCenter = (dx*dx + dy*dy);
}