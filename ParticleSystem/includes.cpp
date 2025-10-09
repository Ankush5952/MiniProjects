#pragma once
#include<raylib.h>
#include<raymath.h>
#include<vector>

static int WIDTH = 900;
static int HEIGHT = 900;
const static char* TITLE = "PARTICLE SYSTEM";
static bool isFullscreen = false;

static Vector2 center = { WIDTH / 2.0F, HEIGHT / 2.0f };

static int maxW;
static int maxH;

static int widthBeforeFS = WIDTH;
static int heightBeforeFS = HEIGHT;

static int prevW = WIDTH;
static int prevH = HEIGHT;

const static int CELLSIZE = 10;
static int GRIDW = WIDTH / CELLSIZE;
static int GRIDH = HEIGHT / CELLSIZE;
static bool isGridEnabled = false;

static void DrawGrid()
{
	for (int i = 0; i < GRIDW; i++)
	{
		for (int j = 0; j < GRIDH; j++)
		{
			DrawRectangleLines(i * CELLSIZE, j * CELLSIZE, CELLSIZE, CELLSIZE, BLACK);
		}
	}
}

static void InitializeRuntimeVariables()
{
	int monitor = GetCurrentMonitor();
	maxW = GetMonitorWidth(monitor);
	maxH = GetMonitorHeight(monitor);
}