#ifndef SLIDER_H
#define SLIDER_H

#include"raylib.h"
#include"includes.cpp"

#pragma once
class Slider
{
private:
	int height;
	int width;
	Color color;
	pair<int, int> pos;
	int slideSpeed;
	int score = 0;

public:
	Slider(int h = 60, int w = 20, Color c = WHITE, pair<int,int> p = {0,0}, int v = 40) : 
		height(h), width(w), color(c), pos(p), slideSpeed(v){ };

	void DrawSlider();
	void setPos(int x, int y);
	pair<int, int> getDimensions();
	int getSpeed();
	pair<int, int> getPos();
	int getScore();
	void setScore(int s);
};
#endif

