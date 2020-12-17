#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const Vector2<int> LEFT(-1,0);
const Vector2<int> RIGHT(1,0);
const Vector2<int> UP(0,-1);
const Vector2<int> DOWN(0,1);

const int SCREEN_WIDTH = 1120;
const int SCREEN_HEIGHT = 1340;
const int MAZE_WIDTH = SCREEN_WIDTH;
const int MAZE_HEIGHT = SCREEN_HEIGHT - 80;
const int FPS = 60;
const int GRID_SIZE = 40;
const int CHARACTER_SPEED = 4;

const int PAC_SIZE = 60;

const Clock program_clock;

#endif 
