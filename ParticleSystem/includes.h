#pragma once
#include<raylib.h>
#include<raymath.h>
#include<vector>

extern int WIDTH;
extern int HEIGHT;
extern const char* TITLE;
extern bool isFullscreen;

extern Vector2 center;

extern int maxW;
extern int maxH;

extern int widthBeforeFS;
extern int heightBeforeFS;

extern int prevW;
extern int prevH;

extern int shapeIndex;
extern int collissionIndex;
extern int particleLifetime;
extern int particleSize;
extern Color colorPresets[];
extern const char* colors[];
extern int colorIndex;

extern const int CELLSIZE;
extern int GRIDW;
extern int GRIDH;
extern bool isGridEnabled;

extern Vector2 gravity;
extern bool gravityEnabled;

extern bool showControls;

extern const double oneOverRoot3;

extern void DrawGrid();

extern void InitializeRuntimeVariables();