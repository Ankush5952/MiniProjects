#include"includes.h"

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

const int CELLSIZE = 20;
int GRIDW = WIDTH / CELLSIZE;
int GRIDH = HEIGHT / CELLSIZE;
bool isGridEnabled = false;

int shapeIndex = 0;
int collissionIndex = 0;
float particleLifetime = 5.0f;
int particleSize = 3;
std::vector<Color> colorPresets = {  WHITE,   RED,   DARKBLUE,   GREEN,   YELLOW,   GRAY,   BROWN,  ORANGE,  PINK , PURPLE };
const char* colors[] = { "WHITE", "RED", "D.BLUE", "GREEN", "YELLOW", "GRAY", "BROWN","ORANGE","PINK","PURPLE" };
Vector2 velocityRangeX = {-50.0f, 50.0f};
Vector2 velocityRangeY = {-50.0f, 50.0f};
int colorIndex = 0;
bool fadeEffect = true;
bool boundary = true;
std::string currentPresetName = "NONE";

Vector2 gravity = { 0, 0 };
bool gravityEnabled = false;

bool showControls = true;

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

