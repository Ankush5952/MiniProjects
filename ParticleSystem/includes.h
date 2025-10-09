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

extern const int CELLSIZE;
extern int GRIDW;
extern int GRIDH;
extern bool isGridEnabled;

extern void DrawGrid();

extern void InitializeRuntimeVariables();