#pragma once
#ifndef BALL_H
#define BALL_H

#include<raylib.h>
#include"includes.cpp"
class Ball
{
private:
    int radius;
    pair<int, int> velocity;
    pair<int, int> pos;
    Color color;
public:
    Ball(int r = 10, pair<int, int> v = { 5,5 }, pair<int, int> p = { 0,0 }, Color c = BLUE) : radius(r), velocity(v), pos(p), color(c) {};

    void DrawBall();
    pair<int, int> getSpeed();
    pair<int, int> getPos();
    int getRadius();
    void setPos(int x, int y);
    void setVelocity(int vx, int vy);
    Color getColor();

};
#endif // !BALL_H



