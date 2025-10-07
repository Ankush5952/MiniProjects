#pragma once
#include<raylib.h>
#include<raymath.h>
#include<vector>

using namespace std;

static int WIDTH = 900;
static int HEIGHT = 900;
static char TITLE[12] = "2D PHYSICS";

static Vector2 gravity = { 0,980 };

static int errors = 0;

static void LogCollissionErrors()
{
	char text[4];
	sprintf_s(text, "%i", errors);
	DrawText(text, WIDTH / 2, 50, 30, RED);
}