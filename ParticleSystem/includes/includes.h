#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>

#include<raylib.h>
#include<raymath.h>
#include<rlgl.h>
#include<raygui.h>

#include"includes/glad/include/glad/glad.h"
#include<vector>
#include<unordered_set>
#include<fstream>
#include<deque>
#include<string>

#include"json.hpp"
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

extern Font default_font;

extern Shader particleShader;
extern bool isParticleShaderLoaded;
extern int fadeLoc;
extern int glowLoc;
extern int glowIntensityLoc;
extern int mvpLoc;
extern Matrix mvpMat;

extern int posLoc;
extern int scaleLoc;
extern int colorLoc;
extern int lifeLoc;
extern int timeLoc;

extern const int CELLSIZE;
extern int GRIDW;
extern int GRIDH;
extern bool isGridEnabled;

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
extern bool trailEffect;
extern bool glowEffect;
extern float glowIntensity;
extern int maxTrail;
extern bool boundary;
extern std::string currentPresetName;
extern int currentEmitterIndex;
extern std::string currentEmitter;
extern float frequency;


extern Vector2 gravity;
extern bool gravityEnabled;

extern bool showControls;

extern const double oneOverRoot3;

extern void DrawGrid();

extern void InitializeRuntimeVariables();