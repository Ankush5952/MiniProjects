#pragma once
#include<raylib.h>
#include<raymath.h>
#include<vector>

using namespace std;

static int WIDTH = 900;
static int HEIGHT = 900;
static char TITLE[12] = "2D PHYSICS";
static bool isFullscreen = false;

static float prevW = WIDTH;
static float prevH = HEIGHT;

static float maxW;
static float maxH;

static float widthBeforeFS;
static float heightBeforeFS;

static Vector2 gravity = { 0,980 };

static int errors = 0;

static Vector2 floorpos{ WIDTH / 2.0f, HEIGHT - 1.0f };
static float floord1 = WIDTH;
static float floord2 = 1.0f;
static Vector2 rwallpos{ 1.0f,         HEIGHT / 2.0f };
static Vector2 lwallpos{ WIDTH - 1.0f, HEIGHT / 2.0f };
static float walld1 = 1.0f;
static float walld2 = HEIGHT;
static Vector2 ceilpos { WIDTH / 2.0f, 2.0f };

static void LogCollissionErrors()
{
	char text[4];
	sprintf_s(text, "%i", errors);
	DrawText(text, WIDTH / 2, 50, 30, RED);
}

static void updateWallParameters()
{
	float w = GetScreenWidth(), h = GetScreenHeight();

	if (isFullscreen)
	{
		w = maxW;
		h = maxH;
	}

	floorpos = { w / 2.0f, h - 1.0f };
	rwallpos = { 1.0f,         h / 2.0f };
	lwallpos = { w - 1.0f, h / 2.0f };
	ceilpos = { w / 2.0f, 2.0f };

	floord1 = w;
	walld2 = h;
}

static void createWindowVariables()
{
	int monitor = GetCurrentMonitor();
	maxW = GetMonitorWidth(monitor);
	maxH = GetMonitorHeight(monitor);
}