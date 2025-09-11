#include <iostream>
#include<raylib.h>
#include"ball.h"
#include"slider.h"

const int WIDTH = 900;
const int HEIGHT = 900;

Ball a;
Slider p1;
Slider p2;

bool paused = false;

float elapsedTime = 0;
float lastElapsedTime = 0;
int i = 1;

void MoveSlider(Slider& s, bool up)
{
    int v = s.getSpeed();
    int posy = s.getPos().second;
    int increment = (up) ? -v : v;
    if (posy + increment < 0) return;
    int newpos = posy + increment;
    newpos = min(newpos, HEIGHT - s.getDimensions().second);
    s.setPos(s.getPos().first, newpos);
}

void SliderAI(Slider& s, Ball& b)
{
    pair<int, int> ballPos = b.getPos();
    pair<int, int> currPos = s.getPos();
    pair<int, int> ballVel = b.getSpeed();

    if ((ballVel.first < 0 && currPos.first == 0) || (ballVel.first > 0 && currPos.first > 0))
    {
        int yPos = (abs(ballVel.second) * (ballPos.first - currPos.first)) / abs(ballVel.first);
        yPos += ballPos.second;

        if (currPos.second + s.getDimensions().second / 2 == yPos) return;

        bool up = currPos.second + s.getDimensions().second/2 > yPos;

        if(s.getPos().second != yPos)
        {
            MoveSlider(s, up);
        }
    }
}

void MoveBall(Ball& b, Slider& s1, Slider& s2, int scale = 1)
{
    pair<int, int> pos = b.getPos();
    pair<int, int> vel = b.getSpeed();
    int radius = b.getRadius();

    int incx = vel.first * scale;
    int incy = vel.second * scale;
    int currx = pos.first, curry = pos.second;

    int nx = currx + incx, ny = curry + incy;

    int leftsliderx = s1.getDimensions().first;
    int rightsliderx = WIDTH - s2.getDimensions().first;
    int leftslidery1 = s1.getPos().second;
    int leftslidery2 = leftslidery1 + s1.getDimensions().second;
    int rightslidery1 = s2.getPos().second;
    int rightslidery2 = rightslidery1 + s2.getDimensions().second;

    bool leftslidercollision = ny >= leftslidery1 && ny <= leftslidery2;
    bool rightslidercollision = ny >= rightslidery1 && ny <= rightslidery2;

    if (leftslidercollision && nx < radius + leftsliderx + 2 && vel.first < 0)
    {
        nx = radius + leftsliderx + 2;
        vel.first = -vel.first;
    }
    if (rightslidercollision && nx > rightsliderx - radius - 2 && vel.first > 0)
    {
        nx = rightsliderx - radius - 2;
        vel.first = -vel.first;
    }
    if (ny < radius || ny > HEIGHT - radius) vel.second = -vel.second;

    if (!leftslidercollision && nx < 0) s2.setScore(s2.getScore() + 1), nx = WIDTH/2, ny = HEIGHT/2, vel.first = -vel.first;
    if (!rightslidercollision && nx > WIDTH - radius) s1.setScore(s1.getScore() + 1), nx = WIDTH / 2, ny = HEIGHT / 2, vel.first = -vel.first;

    b.setVelocity(vel.first, vel.second);
    b.setPos(nx, ny);

    if (elapsedTime > 15 * i) b.setVelocity(vel.first * 1.2, vel.second * 1.2), i++;
}

void DisplayScore(Slider& s1, Slider& s2)
{
    char score1[5];
    char score2[5];
    sprintf_s(score1, "%i", s1.getScore());
    sprintf_s(score2, "%i", s2.getScore());
    DrawText(score1, 50, 0, 20, BLUE);
    DrawText(score2, WIDTH - 50, 0, 20, RED);
}

void Reset()
{
    a.setPos(WIDTH / 2, HEIGHT / 2);
}
void Restart()
{
    p1.setPos(0, 0);
    p2.setPos(WIDTH - 20, 0);
    p1.setScore(0);
    p2.setScore(0);
    lastElapsedTime = GetTime();
    paused = false;
}

void DrawCenteredText(const char* text, int y, int fontSize, Color color)
{
    int textWidth = MeasureText(text, fontSize);
    int x = (GetScreenWidth() - textWidth) / 2;
    DrawText(text, x, y, fontSize, color);
}


int main()
{
    InitWindow(WIDTH, HEIGHT, "PONG");
    SetTargetFPS(60);
    a = { 20, { -6,-5 }, { WIDTH / 2,HEIGHT / 2 }, WHITE };

    p1 = { 100, 20, BLUE, { 0,          0}, 20 };
    p2 = { 100, 20, RED, { WIDTH - 20, 0}, 20 };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        if(!paused)
        {
            elapsedTime = GetTime();
            cout << elapsedTime << '\n';

            ClearBackground(GREEN);

            a.DrawBall();
            MoveBall(a, p1, p2);

            p1.DrawSlider();
            p2.DrawSlider();

            if (IsKeyDown(KEY_W)) MoveSlider(p1, true);
            if (IsKeyDown(KEY_S)) MoveSlider(p1, false);
            if (IsKeyPressed(KEY_M)) p1.setScore(5);
            if (IsKeyPressed(KEY_L)) p2.setScore(5);

            SliderAI(p2, a);

            /*if (IsKeyDown(KEY_UP)) MoveSlider(p2, true);
            if (IsKeyDown(KEY_DOWN)) MoveSlider(p2, false);*/

            DisplayScore(p1, p2);

            char time[10];
            float acttime = (elapsedTime - lastElapsedTime);
            int inttime = acttime;
            int floattime = 100 * (acttime - inttime);
            sprintf_s(time, "%i . %d", inttime, floattime);
            DrawCenteredText(time, 0, 20, YELLOW);

            if(acttime < 20) DrawCenteredText("Survive 100s to beat the AI", 20, 30, GRAY);

            if (p1.getScore() >= 5 || p2.getScore() >= 5)
            {
                Reset();
                paused = true;
            }

        }
        else
        {
            char text[10];
            char score[100];
            sprintf_s(text, (p1.getScore() >= 5) ? "YOU WON" : "YOU LOST");
            DrawCenteredText(text, HEIGHT / 2 - 20, 50, WHITE);

            if (p1.getScore() < 5) sprintf_s(score, "You Survived %i s", (int)(elapsedTime - lastElapsedTime));
            else sprintf_s(score, "Congratulations, You managed to survuve %i s", (int)(elapsedTime - lastElapsedTime));
            DrawCenteredText(score, HEIGHT / 2 + 50, 15, WHITE);

            char msg[20] = "Press R to Restart";
            DrawCenteredText(msg, HEIGHT / 2 + 80, 15, RED);
            if (IsKeyPressed(KEY_R)) Restart();
        }

        EndDrawing();
    }
    CloseWindow();
}