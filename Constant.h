/// Constant.h
#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include<string>
#include<iostream>
#include<vector>
#include<cmath>
#include<array>
#include<stack>
#include<queue>
#include <random>
#include <chrono>

using namespace sf;
using namespace std; 

const int Initial_main = -1; 
const int GamePlay_main = 1;
const int Settings_main = 2;
const int TwoPlayers_main = 3;
const int OnePlayer_main = 4;
const int SaveGame_main = 5; 
const int Playasblack_main = 6; 
const int Playaswhite_main = 7; 

const int king_main = 0;
const int queen_main = 1;
const int bishop_main = 2;
const int knight_main = 3;
const int rook_main = 4;
const int pawn_main = 5;

const int SIZE = 75; 

const int Kx[] = { -1, 1, -1, 1, 0, 0, -1, 1 };
const int Ky[] = { -1, 1, 1, -1, -1, 1, 0, 0 };

const int Knx[] = { -1, -2, 1, 2, 2, 1, -1, -2 };
const int Kny[] = { -2, -1, -2, -1, 1, 2, 2, 1 };

extern std::mt19937 rd;
int random(int l, int r);   

class Figure {
public:
    Figure();
    Figure(int t, Texture& path);
    void SetPosition(int x, int y);
    void Setposition(Vector2i pos);
    Sprite shape;
    int type;
    int played;
};
