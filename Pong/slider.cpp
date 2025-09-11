#include "slider.h"
#include "ball.h"

void Slider::DrawSlider()
{
    DrawRectangle(pos.first, pos.second, width, height, color);
}

void Slider::setPos(int x, int y)
{
    pos = { x,y };
}
pair<int, int> Slider::getDimensions()
{
    return{ width, height };
}

int Slider::getSpeed()
{
    return slideSpeed;
}
pair<int, int> Slider::getPos()
{
    return pos;
}
int Slider::getScore()
{
    return score;
}
void Slider::setScore(int s)
{
    score = s;
}