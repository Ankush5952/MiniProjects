#pragma once
#include<raylib.h>
#include<raymath.h>
#include<vector>
#include<unordered_set>
#include<fstream>
#include"json.hpp"
#include<string>
#include"game_events.h"


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
extern float particleLifetime;
extern int particleSize;
extern std::vector<Color> colorPresets;
extern const char* colors[];
extern Vector2 velocityRangeX;
extern Vector2 velocityRangeY;
extern int colorIndex;
extern bool fadeEffect;
extern bool boundary;
extern std::string currentPresetName;
extern int currentEmitterIndex;
extern std::string currentEmitter;
extern float frequency;

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