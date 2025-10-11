#include"particle_manager.h"

int WIDTH = 900;
int HEIGHT = 900;
const char* TITLE = "Particle System";
bool isFullscreen = false;

Vector2 center = { WIDTH / 2.0F, HEIGHT / 2.0f };

int maxW;
int maxH;

int widthBeforeFS = WIDTH;
int heightBeforeFS = HEIGHT;

int prevW = WIDTH;
int prevH = HEIGHT;

const int CELLSIZE = 10;
int GRIDW = WIDTH / CELLSIZE;
int GRIDH = HEIGHT / CELLSIZE;
bool isGridEnabled = false;

int shapeIndex = 0;
int collissionIndex = 0;

const double oneOverRoot3 = 1.0 / sqrt(3.0);

void DrawGrid()
{
	for (int i = 0; i < GRIDW; i++)
	{
		for (int j = 0; j < GRIDH; j++)
		{
			DrawRectangleLines(i * CELLSIZE, j * CELLSIZE, CELLSIZE, CELLSIZE, BLACK);
		}
	}
}

void InitializeRuntimeVariables()
{
	int monitor = GetCurrentMonitor();
	maxW = GetMonitorWidth(monitor);
	maxH = GetMonitorHeight(monitor);
}

