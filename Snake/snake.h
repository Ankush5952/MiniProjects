#pragma once
#ifndef SNAKE_H
#define SNAKE_H

#include"includes.cpp"

class Snake
{
public:
	int headx;
	int heady;
	vector<pair<int, int>> tail = {};

	Snake(int Headx = 0, int Heady = 1) : headx(Headx), heady(Heady) {};
};

#endif // !SNAKE_H


