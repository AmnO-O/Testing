#pragma once
#include <iostream>
#include<cstring>
#include <array>
#include <vector>
#include <utility> 
#include "Constant.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

#define pii pair<int,int>

struct Pieces {
	int type; 
	int x, y; 
	int played; 
	Pieces() {}
	Pieces(int a, int b, int c, int d) { type = a, x = b, y = c; played = d; }
};

class AI{
public:
	AI();
	int getEvaluate(); 
	void MakeMove(int x, int y); 
	pair<int,pii> decision();

	void UpdHistoryWhite(Pieces A, int x, int y);
	void UpdHistoryBlack(Pieces A, int x, int y);
	bool KingSafe(pii temp);
	void getValidMove(Pieces A, vector<pii>& move);
	void UpdBoardState();
	void PawnLogic(Pieces A, vector<pii>& move);
	void KingLogic(Pieces A, vector <pii>& move);
	void KnightLogic(Pieces A, vector<pii>& move);
	void Horizontal(Pieces A, vector<pii>& move);
	void Vertical(Pieces A, vector<pii>& move);
	void Diagonal(Pieces A, vector<pii>& move);
	bool TakePiece(int x, int y);
	void Castling(vector <pii>& move);
	bool check(); 
	int dfs(int depth); 
	void RollBack(); 
	void Print(); 


	int BoardState[10][10];
	int Occur[10][10];
	Pieces Black[16];
	Pieces White[16];
private:
	vector <array<int, 4>> Black_history, White_history;
	bool Turn; 
	int numberOfmove; 
	array<int, 4> store[1001][33];
	int value[1000];
	int index;
	pair<int, pair<int, int>> lst_search; 
};