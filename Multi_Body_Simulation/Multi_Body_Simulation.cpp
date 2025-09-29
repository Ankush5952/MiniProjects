#include<raylib.h>
#include"includes.cpp"

Color colors[] = {RED, BLUE, GREEN, YELLOW, GRAY, ORANGE, PINK, PURPLE, VIOLET, BROWN};

//const float G = 6.674e-5;
const float G = 1000.0f;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dist(0, 9);

struct Body
{
	float mass;
	float radius;
	float posx;
	float posy;
	float velx = 0;
	float vely = 0;
	Color color = colors[dist(gen)];

	vector<Vector2> trail = {{ posx, posy }};

	void AddTrailPoint()
	{
		if(posx != trail[0].x && posy != trail[0].y)
		{
			trail.push_back({ posx, posy });
		}
	}

	void DrawOrbit()
	{
		if (trail.size() < 2) return;

		for (int i = 1; i < trail.size(); i++)
		{
			DrawLine(trail[i - 1].x, trail[i - 1].y, trail[i].x, trail[i].y, { color.r, color.b, color.g, 100 });
		}
	}

	float getg()
	{
		return (G * mass) / (radius * radius);
	}
};

void DrawBody(Body& b)
{
	DrawCircle((int)b.posx, (int)b.posy, b.radius, b.color);
}

float CalculateForce(Body& a, Body& b, float sqauredist)
{
	//if (sqauredist < 1.0f) return 0;
	return (G * a.mass * b.mass) / sqauredist;
}

void UpdateVelocity(Body& b, float fx, float fy, float dt)
{
	float ax = fx / b.mass;
	float ay = fy / b.mass;

	b.velx += ax * dt;
	b.vely += ay * dt;
}

void MoveBody(Body& b, float dt)
{
	float xi = b.posx;
	float yi = b.posy;
	b.posx += b.velx * dt;
	b.posy += b.vely * dt;
	b.AddTrailPoint();
}

void ApplyForces(Body& a, Body& b)
{
	float dx = b.posx - a.posx;
	float dy = b.posy - a.posy;
	float r2 = dx * dx + dy * dy;
	float r = sqrt(r2);
	if (r < 1.0f || r <= a.radius + b.radius) return;
	float Force = CalculateForce(a, b, r2);

	float fx = Force * (dx / r);
	float fy = Force * (dy / r);

	float dt = GetFrameTime();

	UpdateVelocity(a, fx, fy, dt);
	UpdateVelocity(b, -fx, -fy, dt);

	MoveBody(a, dt);
	MoveBody(b, dt);
}

int WIDTH = 1600;
int HEIGHT = 1000;
char TITLE[24] = "Multi Body Simulation";

int main()
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(60);

	Body A{ 100,    10,  WIDTH/2 + 300,   HEIGHT/2,     0,   -500,  RED};
	Body B{ 100,    10,  WIDTH / 2 - 220, HEIGHT / 2 ,  0,    500,  BLUE};
	Body C{ 100000, 50,  WIDTH/2,         HEIGHT / 2,   0,    0,  YELLOW};

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		ApplyForces(A, B);
		ApplyForces(A, C);
		ApplyForces(B, C);
		DrawBody(A);
		DrawBody(B);
		DrawBody(C);
		A.DrawOrbit();
		B.DrawOrbit();
		EndDrawing();
	}
	CloseWindow();
}