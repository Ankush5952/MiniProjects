#include"ball.h"

void Ball::DrawBall()
{
    DrawCircle(pos.first, pos.second, radius, color);
}
void Ball::setVelocity(int vx, int vy)
{
    velocity = { vx, vy };
}

pair<int, int> Ball::getPos()
{
    return pos;
}

int Ball::getRadius()
{
    return radius;
}

void Ball::setPos(int x, int y)
{
    pos = { x,y };
}

pair<int,int> Ball::getSpeed()
{
    return velocity;
}
Color Ball::getColor()
{
    return color;
}
