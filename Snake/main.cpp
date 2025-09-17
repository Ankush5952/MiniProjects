#include"raylib.h"
#include"includes.cpp"
#include "snake.h"
#include"apple.h"

enum DIRECTIONS {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

const int WIDTH = 900;
const int HEIGHT = 900;
char TITLE[6] = "Snake";

const int CELLSIZE = 20;
int GRIDX = WIDTH / CELLSIZE;
int GRIDY = HEIGHT / CELLSIZE;

bool up = false;
bool down = false;
bool l = false;
bool r = true;

int score = 0;

Snake player;
Apple apple;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> indexx(0, GRIDX - 1);
uniform_int_distribution<int> indexy(1, GRIDY-1);

float delay = 0.15f;
float lastMoved = 0.0f;

bool paused = false;

void DrawCenteredText(const char* text, int y, int fontSize, Color color)
{
	int textWidth = MeasureText(text, fontSize);
	int x = (GetScreenWidth() - textWidth) / 2;
	DrawText(text, x, y, fontSize, color);
}

void GameOver()
{
	paused = true;
}

bool isTail(int x, int y)
{
	for (auto i : player.tail)
	{
		if (i.first == x && i.second == y) return true;
	}
	return false;
}

void DrawCells()
{
	DrawRectangle(0, 0, GRIDX * CELLSIZE, CELLSIZE, DARKBROWN);
	for (int i = 0; i < GRIDX; i++)
	{
		for (int j = 1; j < GRIDY; j++)
		{
			DrawRectangle(i * CELLSIZE, j * CELLSIZE, CELLSIZE, CELLSIZE, DARKGREEN);
			DrawRectangle((i) * CELLSIZE + 1, (j)*CELLSIZE + 1, CELLSIZE-2, CELLSIZE-2, GREEN);
		}
	}
}

void DrawSnake()
{
	for (auto i : player.tail)
	{
		DrawRectangle(i.first * CELLSIZE, i.second * CELLSIZE, CELLSIZE, CELLSIZE, BLUE);
		DrawRectangleLines(i.first * CELLSIZE, i.second * CELLSIZE, CELLSIZE, CELLSIZE, BLACK);
	}
	DrawRectangle(player.headx * CELLSIZE, player.heady * CELLSIZE, CELLSIZE, CELLSIZE, DARKBLUE);
	DrawRectangleLines(player.headx * CELLSIZE, player.heady * CELLSIZE, CELLSIZE, CELLSIZE, BLACK);
}

void MoveSnake(DIRECTIONS dir)
{

	float now = GetTime();
	if (now - lastMoved < delay) return;
	lastMoved = now;

	for (int i = player.tail.size() - 1; i > 0; i--)
	{
		player.tail[i] = player.tail[i - 1];
	}
	if (!player.tail.empty())player.tail[0] = { player.headx, player.heady };

	int nx = player.headx, ny = player.heady;

	switch (dir)
	{
		case UP:
			ny = (ny == 1) ? GRIDY - 1 : ny - 1;
			break;
		case DOWN:
			ny = (ny == GRIDY-1) ? 1 : ny + 1;
			break;
		case LEFT:
			nx = (nx == 0) ? GRIDX - 1 : nx - 1;
			break;
		case RIGHT:
			nx = (nx == GRIDX-1) ? 0 : nx + 1;
			break;
	}
	if (isTail(nx, ny)) GameOver(); //Game End Logic
	else player.headx = nx, player.heady = ny;
}

void AddTail()
{
	int nx = (!player.tail.empty()) ? player.tail[player.tail.size() - 1].first : player.headx;
	int ny = (!player.tail.empty()) ? player.tail[player.tail.size() - 1].second: player.heady;

	if (up) ny = (ny == GRIDY-1) ? 1 : ny + 1;
	if (down) ny = (ny == 1) ? GRIDY-1 : ny - 1;
	if (l) nx = (nx == GRIDX - 1) ? 0 : nx + 1;
	if (r) nx = (nx == 0) ? GRIDX-1 : nx - 1;

	player.tail.push_back({ nx, ny });

	score++;
}

void DrawApple()
{
	DrawRectangle(apple.x * CELLSIZE, apple.y * CELLSIZE, CELLSIZE, CELLSIZE, RED);
}

void SpawnApple()
{
	int nx = indexx(gen);
	int ny = indexy(gen);
	
	apple.x = nx;
	apple.y = ny;
}

void SetTrue(bool& dir)
{
	up = (&up == &dir) ? true : false;
	down = (&down == &dir) ? true : false;
	l = (&l == &dir) ? true : false;
	r = (&r == &dir) ? true : false;
}

void Restart()
{
	player.headx = 0, player.heady = 1, SpawnApple(), player.tail.clear(), SetTrue(r), score = 0;
	paused = false;
}

void DisplayScore()
{
	char temp[16];
	sprintf_s(temp, "%i", score);

	DrawText(temp, CELLSIZE, 0, CELLSIZE, YELLOW);
}

int main()
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(60);

	SpawnApple();
	while (!WindowShouldClose())
	{
		BeginDrawing();
		if(!paused)
		{
			ClearBackground(BLACK);
			DrawCells();
			DisplayScore();
			DrawApple();
			DrawSnake();

			if (IsKeyPressed(KEY_W)) SetTrue(up);
			if (IsKeyPressed(KEY_S)) SetTrue(down);
			if (IsKeyPressed(KEY_A)) SetTrue(l);
			if (IsKeyPressed(KEY_D)) SetTrue(r);

			if (up) MoveSnake(UP);
			if (down) MoveSnake(DOWN);
			if (l) MoveSnake(LEFT);
			if (r) MoveSnake(RIGHT);

			//Eat Apple
			if (player.headx == apple.x && player.heady == apple.y) { SpawnApple(); AddTail(); }

			//Debug
			if (IsKeyPressed(KEY_N)) AddTail();
			if (IsKeyPressed(KEY_M)) SpawnApple();
			if (IsKeyPressed(KEY_O)) GameOver();
		}
		else
		{
			ClearBackground(BLACK);
			DrawCells();
			DisplayScore();
			DrawApple();
			DrawSnake();

			char scoretext[32];
			sprintf_s(scoretext, "You scored %i apples", score);

			DrawCenteredText("GAME OVER", HEIGHT / 2 - 30, 60, WHITE);
			DrawCenteredText("PRESS R TO RESTART", HEIGHT / 2 + 30, 25, RED);
			DrawCenteredText(scoretext, HEIGHT / 2 + 60, 25, VIOLET);

			if(IsKeyPressed(KEY_R)) Restart();
		}

		EndDrawing();
	}
	CloseWindow();
}
