#include "AI.h"

AI::AI() {
	value[queen_main] = 9;
	value[king_main] = 1000;
	value[bishop_main] = 3;
	value[knight_main] = 3;
	value[pawn_main] = 1;
	value[rook_main] = 5;
	Turn = 1; 
}

void AI::MakeMove(int x, int y) {
	if (Turn == 0) {
		if (White[index].type == pawn_main && y == 0) {
			int x = random(1, 4);

			if (x == rook_main) White[index].type = rook_main;
			if (x == queen_main)  White[index].type = queen_main;
			if (x == bishop_main) White[index].type = bishop_main;
			if (x == knight_main) White[index].type = knight_main;

			TakePiece(x, y);

			UpdHistoryWhite(White[index], x, y);
			White[index].x = x;
			White[index].y = y;
			White[index].played++;

			numberOfmove++;
			UpdBoardState();

			return; 
		}


		if (White[index].played == 0 && index == 4) {
			if (x == White[index].x + 2 && y == White[index].y) White[7].x = x - 1, White[7].y = y, White[7].played++;
			else if (x == White[index].x - 2 && y == White[index].y) White[0].x = x + 1, White[0].y = y, White[0].played++;
		}

		TakePiece(x, y);
		UpdHistoryWhite(White[index], x, y);
		White[index].x = x; 
		White[index].y = y; 
		White[index].played++;
		numberOfmove++;

		UpdBoardState();
		return;
	}


	if (Black[index].type == pawn_main && y == 7) {
		int x = random(1, 4);

		if (x == rook_main)  Black[index].type = rook_main;
		if (x == queen_main) Black[index].type = queen_main;
		if (x == bishop_main) Black[index].type = bishop_main;
		if (x == knight_main) Black[index].type = knight_main;

		TakePiece(x, y);

		UpdHistoryBlack(Black[index], x, y);
		
		Black[index].x = x;
		Black[index].y = y; 
		Black[index].played++;

		numberOfmove++; 
		UpdBoardState();

		return; 
	}

	TakePiece(x, y);

	if (Black[index].played == 0 && index == 4) {
		if (x == Black[index].x + 2 && y == Black[index].y) Black[7].x = x - 1, Black[7].y = y, Black[7].played++;
		else if (x == Black[index].x - 2 && y == Black[index].y) Black[0].x = x + 1, Black[0].y = y, Black[0].played++;
	}


	UpdHistoryBlack(Black[index], x, y);

	numberOfmove++;
	Black[index].x = x; 
	Black[index].y = y; 
	Black[index].played++;

	UpdBoardState();

}

void AI::RollBack() {
	numberOfmove--; 
	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = store[numberOfmove][i];
		White[i].type = lst_move[2];
		White[i].x = lst_move[0]; 
		White[i].y = lst_move[1]; 
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = store[numberOfmove][i + 16];
		Black[i].type = lst_move[2];
		Black [i] .x = lst_move[0];
		Black [i] .y = lst_move[1];
		Black[i].played = lst_move[3];
	}

	UpdBoardState(); 
}

int AI::getEvaluate() {
	int val = 0; 
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (Occur[i][j] == 1) {
				val += value[BoardState[i][j]]; 
			}
			else if(Occur[i][j] == 2){
				val -= value[BoardState[i][j]]; 
			}
		}
	}

	return val; 
}

void AI::Print() {

}

int AI::dfs(int depth) {
	if (depth == 0) 
		return getEvaluate();

	vector<int>indicies;
	vector<pair<int, pair<int, int>>> Random;
	int num = 0; 

	if (Turn) {
		for (int i = 0; i < 16; i++) {
			if (Black[i].x < 0 || Black[i].y < 0) continue;
			indicies.push_back(i);
		}

		shuffle(indicies.begin(), indicies.end(), rd);


		while (indicies.size()) {
			num++; 
			if (num == 6) break; 
			vector<pair<int,int>> valid;

			index = indicies.back();
			getValidMove(Black[indicies.back()], valid);

			if (Turn == 0 && White[index].type == king_main && White[index].played == 0 && KingSafe(pii(White[index].x, White[index].y))) {
				Castling(valid);
			}
			else if (Turn && Black[index].type == king_main && Black[index].played == 0 && KingSafe(pii(Black[index].x, Black[index].y))) {
				Castling(valid);
			}

			shuffle(valid.begin(), valid.end(), rd);
			int cnt = 0; 

			for (int i = 0; i < valid.size() && cnt < 6; i++) if(KingSafe(valid[i])){
				cnt++; 
				Random.push_back(make_pair(index, valid[i]));
			}

			indicies.pop_back();
		}

		pair <int, pair<int, int>> mx; int mxVal = -1e9;

		for (auto X : Random) {
			index = X.first; 
			
			int x = X.second.first; 
			int y = X.second.second;
			
			MakeMove(x, y); 	
			Turn = 0; 
			
			int c = dfs(depth - 1); 

			if (mxVal < c) {
				mxVal = c; 
				mx = { X.first, {x, y} };
			}

			Turn = 1; 
			RollBack(); 
		}

		lst_search = mx; 

		return mxVal; 
	}

	for (int i = 0; i < 16; i++) {
		if (White[i].x < 0 || White[i].y < 0) continue;
		indicies.push_back(i);
	}
	shuffle(indicies.begin(), indicies.end(), rd);

	while (indicies.size()) {
		num++; 
		if (num == 6) break; 
		vector<pair<int, int>> valid;
		getValidMove(Black[indicies.back()], valid);
		index = indicies.back();


		if (Turn == 0 && White[index].type == king_main && White[index].played == 0 && KingSafe(pii(White[index].x, White[index].y))) {
			Castling(valid);
		}
		else if (Turn && Black[index].type == king_main && Black[index].played == 0 && KingSafe(pii(Black[index].x, Black[index].y))) {
			Castling(valid);
		}

		shuffle(valid.begin(), valid.end(), rd);
		int cnt = 0; 

		for (int i = 0; i < valid.size() && cnt < 6; i++) if (KingSafe(valid[i])) {
			cnt++;
			Random.push_back(make_pair(index, valid[i]));
		}

		indicies.pop_back();
	}

	int mnVal = 1e9;

	for (auto X : Random) {
		index = X.first;

		int x = X.second.first;
		int y = X.second.second;

		MakeMove(x, y);
		Turn = 1; 

		mnVal = min(mnVal, dfs(depth - 1)); 

		Turn = 0; 
		RollBack();
	}
	
	return mnVal; 
}

pair<int,pii> AI::decision() {
	dfs(6);
	return lst_search;
}

void AI::PawnLogic(Pieces A, vector<pii>& move) {
	int x = A.x;
	int y = A.y;

	if (Turn == 0) {
		if (y > 0 && Occur[x][y - 1] == 0) {
			move.push_back(pii(x, y - 1));
		}

		if (y >= 2 && A.played == 0 && Occur[x][y - 2] == 0 && Occur[x][y - 1] == 0) {
			move.push_back(pii(x, y - 2));
		}

		if (y > 0 && x < 7 && Occur[x][y] != Occur[x + 1][y - 1] && Occur[x + 1][y - 1] > 0) {
			move.push_back(pii(x + 1, y - 1));
		}

		if (y > 0 && x > 0 && Occur[x][y] != Occur[x - 1][y - 1] && Occur[x - 1][y - 1] > 0) {
			move.push_back(pii(x - 1, y - 1));
		}

		if (x < 7 && y > 0) {
			if (Black_history.size() && Black_history.back()[3] == pawn_main && Black[Black_history.back()[2]].played == 1) {
				int lst_x = Black_history.back()[0];
				int lst_y = Black_history.back()[1];

				if (lst_x == x + 1 && lst_y == y) move.push_back(pii(x + 1, y - 1));
			}
		}

		if (x > 0 && y > 0) {
			if (Black_history.size() && Black_history.back()[3] == pawn_main && Black[Black_history.back()[2]].played == 1) {
				int lst_x = Black_history.back()[0];
				int lst_y = Black_history.back()[1];

				if (lst_x == x - 1 && lst_y == y) move.push_back(pii(x - 1, y - 1));
			}
		}

		return;
	}

	if (y < 7 && BoardState[x][y + 1] == 0) {
		move.push_back(pii(x, y + 1));
	}

	if (y <= 5 && A.played == 0 && BoardState[x][y + 2] == 0 && BoardState[x][y + 1] == 0) {
		move.push_back(pii(x, y + 2));
	}

	if (y < 7 && x < 7 && Occur[x][y] != Occur[x + 1][y + 1] && Occur[x + 1][y + 1] > 0) {
		move.push_back(pii(x + 1, y + 1));
	}

	if (y < 7 && x > 0 && Occur[x][y] != Occur[x - 1][y + 1] && Occur[x - 1][y + 1] > 0) {
		move.push_back(pii(x - 1, y + 1));
	}

	if (x < 7 && y < 7) {
		if (White_history.size() && White_history.back()[3] == pawn_main && White[White_history.back()[2]].played == 1) {
			int lst_x = White_history.back()[0];
			int lst_y = White_history.back()[1];

			if (lst_x == x + 1 && lst_y == y) move.push_back(pii(x + 1, y + 1));
		}
	}

	if (x > 0 && y < 7) {
		if (White_history.size() && White_history.back()[3] == pawn_main && White[White_history.back()[2]].played == 1) {
			int lst_x = White_history.back()[0];
			int lst_y = White_history.back()[1];

			if (lst_x == x - 1 && lst_y == y) move.push_back(pii(x - 1, y + 1));
		}
	}
}

void AI::KingLogic(Pieces A, vector <pii>& move) {
	int u = A.x; int v = A.y; 

	for (int i = 0; i < 8; i++) {
		int x = u + Kx[i];
		int y = v + Ky[i];
		if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[u][v]) continue;
		move.push_back(pii(x, y));
	}
}

void AI::KnightLogic(Pieces A, vector<pii>& move) {
	int u = A.x; int v = A.y;

	for (int i = 0; i < 8; i++) {
		int x = u + Knx[i];
		int y = v + Kny[i];
		if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[u][v]) continue;
		move.push_back(pii(x, y));
	}
}

void AI::Horizontal(Pieces A, vector<pii>& move) {
	int x = A.x; 
	int y = A.y; 

	for (int i = x + 1; i < 8; i++) {
		if (Occur[x][y] == Occur[i][y]) break;
		move.push_back(pii(i, y));
		if (Occur[i][y] > 0) break;
	}
	for (int i = x - 1; i >= 0; i--) {
		if (Occur[x][y] == Occur[i][y]) break;
		move.push_back(pii(i, y));
		if (Occur[i][y] > 0) break;
	}
}

void AI::Vertical(Pieces A, vector<pii>& move) {
	int x = A.x;
	int y = A.y; 

	if (x < 0 || y < 0) return;
	for (int i = y + 1; i < 8; i++) {
		if (Occur[x][y] == Occur[x][i]) break;
		move.push_back(pii(x, i));
		if (Occur[x][i] > 0) break;
	}
	for (int i = y - 1; i >= 0; i--) {
		if (Occur[x][y] == Occur[x][i]) break;
		move.push_back(pii(x, i));
		if (Occur[x][i] > 0) break;
	}
}

void AI::Diagonal(Pieces A, vector<pii>& move) {
	int x = A.x;
	int y = A.y;

	if (x < 0 || y < 0) return;

	int i, j; i = x, j = y;

	while (true) {
		i--; j--;
		if (i < 0 || j < 0) break;
		if (Occur[i][j] != Occur[x][y]) {
			move.push_back(pii(i, j));
		}

		if (Occur[i][j] > 0) break;
	}

	i = x, j = y;

	while (true) {
		i--; j++;
		if (i < 0 || j > 7) break;
		if (Occur[i][j] != Occur[x][y]) {
			move.push_back(pii(i, j));
		}
		if (Occur[i][j] > 0) break;
	}


	i = x, j = y;

	while (true) {
		i++; j--;
		if (i > 7 || j < 0) break;
		if (Occur[i][j] != Occur[x][y]) {
			move.push_back(pii(i, j));
		}
		if (Occur[i][j] > 0) break;
	}

	i = x, j = y;

	while (true) {
		i++; j++;
		if (i > 7 || j > 7) break;
		if (Occur[i][j] != Occur[x][y]) {
			move.push_back(pii(i, j));
		}
		if (Occur[i][j] > 0) break;
	}
}

void AI::getValidMove(Pieces A, vector<pii>& move) {

	if (A.type == pawn_main) {
		PawnLogic(A, move);
		return;
	}

	if (A.type == knight_main) {
		KnightLogic(A, move);
		return;
	}

	if (A.type == rook_main) {
		Vertical(A, move);
		Horizontal(A, move);

		return;
	}

	if (A.type == bishop_main) {
		Diagonal(A, move);
		return;
	}

	if (A.type == king_main) {
		KingLogic(A, move);
		return;
	}

	Vertical(A, move);
	Horizontal(A, move);
	Diagonal(A, move);
}

bool AI::TakePiece(int x, int y) { 

	if (Turn == 0 && Black_history.size() && Black_history.back()[3] == pawn_main && White[index].type == pawn_main) {
		int lst_x = Black_history.back()[0];
		int lst_y = Black_history.back()[1];
		int id = Black_history.back()[2];

		if (Black[id].played == 1 && lst_x == x && lst_y == y + 1) {
			Black[id].x = -400; 
			Black[id].y = -400;
			return 1;
		}
	}
	else if (Turn && White_history.size() && White_history.back()[3] == pawn_main && Black[index].type == pawn_main) {
		int lst_x = White_history.back()[0];
		int lst_y = White_history.back()[1];
		int id = White_history.back()[2];

		if (White[id].played == 1 && lst_x == x && lst_y == y - 1) {
			White[id].x = -400;
			White[id].y = -400;			
			return 1;
		}
	}

	for (int i = 0; i < 16; i++) {
		if (White[i].x == x && White[i].y == y) {
			White[i].x = -400;
			White[i].y = -400;
			return 1;
		}
	}

	for (int i = 0; i < 16; i++) {
		if (Black[i].x == x && Black[i].y == y) {
			Black[i].x = -400;
			Black[i].y = -400;
			return 1;
		}
	}

	return 0;
}

void AI::UpdBoardState() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			BoardState[i][j] = 0;
			Occur[i][j] = 0;
		}
	}

	for (int i = 0; i < 16; i++) {
		store[numberOfmove][i] = { White[i].x, White[i].y, White[i].type, White[i].played };
		int x = White[i].x;
		int y = White[i].y;
		if (x < 0 || y < 0) continue;
		BoardState[x][y] = White[i].type + 100;
		Occur[x][y] = 2;
	}

	for (int i = 0; i < 16; i++) {
		store[numberOfmove][i + 16] = { Black[i].x, Black[i].y, Black[i].type, Black[i].played };
		int x = Black[i].x; 
		int y = Black[i].y; 
		if (x < 0 || y < 0) continue;
		BoardState[x][y] = Black[i].type + 200;
		Occur[x][y] = 1;
	}
}

bool AI::check() {
	if (Turn == 0) {
		int x = White[4].x;
		int y = White[4].y;

		if (x < 0 || y < 0) return 1;

		for (int i = x - 1; i >= 0; i--) {
			if (Occur[i][y] == 0) continue;
			if (Occur[i][y] == Occur[x][y]) break;
			if (BoardState[i][y] == queen_main + 200 || BoardState[i][y] == rook_main + 200) return 1;
			if (Occur[i][y] > 0) break;
		}

		for (int i = x + 1; i <= 7; i++) {
			if (Occur[i][y] == 0) continue;
			if (Occur[i][y] == Occur[x][y]) break;
			if (BoardState[i][y] == queen_main + 200 || BoardState[i][y] == rook_main + 200) return 1;
			if (Occur[i][y] > 0) break;
		}

		for (int i = y + 1; i <= 7; i++) {
			if (Occur[x][i] == 0) continue;
			if (Occur[x][i] == Occur[x][y]) break;
			if (BoardState[x][i] == queen_main + 200 || BoardState[x][i] == rook_main + 200) return 1;
			if (Occur[x][i] > 0) break;
		}

		for (int i = y - 1; i >= 0; i--) {
			if (Occur[x][i] == 0) continue;
			if (Occur[x][i] == Occur[x][y]) break;
			if (BoardState[x][i] == queen_main + 200 || BoardState[x][i] == rook_main + 200) return 1;
			if (Occur[x][i] > 0) break;
		}


		int i, j; i = x, j = y;

		while (true) {
			i--; j--;
			if (i < 0 || j < 0) break;
			if (Occur[i][j] == Occur[x][y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}

		i = x, j = y;

		while (true) {
			i--; j++;
			if (i < 0 || j > 7) break;
			if (Occur[i][j] == Occur[x][y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}


		i = x, j = y;

		while (true) {
			i++; j--;
			if (i > 7 || j < 0) break;
			if (Occur[i][j] == Occur[x][y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}

		i = x, j = y;

		while (true) {
			i++; j++;
			if (i > 7 || j > 7) break;
			if (Occur[i][j] == Occur[x][y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}


		for (int i = 0; i < 8; i++) {
			int u = x - Knx[i];
			int v = y - Kny[i];
			if (u < 0 || v < 0 || u > 7 || v > 7 || Occur[u][v] == Occur[u][v]) continue;
			if (BoardState[x][y] == knight_main + 200) return 1;
		}

		if (x >= 1 && y >= 1 && BoardState[x - 1][y - 1] == pawn_main + 200) return 1;
		if (x < 7 && y >= 1 && BoardState[x + 1][y - 1] == pawn_main + 200) return 1;

		return 0;
	}

	int x = Black[4].x;
	int y = Black[4].y;

	if (x < 0 || y < 0) return 1;

	for (int i = x - 1; i >= 0; i--) {
		if (Occur[i][y] == 0) continue;
		if (Occur[i][y] == Occur[x][y]) break;
		if (BoardState[i][y] == queen_main + 100 || BoardState[i][y] == rook_main + 100) return 1;
		if (Occur[i][y] > 0) break;
	}

	for (int i = x + 1; i <= 7; i++) {
		if (Occur[i][y] == 0) continue;
		if (Occur[i][y] == Occur[x][y]) break;
		if (BoardState[i][y] == queen_main + 100 || BoardState[i][y] == rook_main + 100) return 1;
		if (Occur[i][y] > 0) break;
	}

	for (int i = y + 1; i <= 7; i++) {
		if (Occur[x][i] == 0) continue;
		if (Occur[x][i] == Occur[x][y]) break;
		if (BoardState[x][i] == queen_main + 100 || BoardState[x][i] == rook_main + 100) return 1;
		if (Occur[x][i] > 0) break;
	}

	for (int i = y - 1; i >= 0; i--) {
		if (Occur[x][i] == 0) continue;
		if (Occur[x][i] == Occur[x][y]) break;
		if (BoardState[x][i] == queen_main + 100 || BoardState[x][i] == rook_main + 100) return 1;
		if (Occur[x][i] > 0) break;
	}


	int i, j; i = x, j = y;

	while (true) {
		i--; j--;
		if (i < 0 || j < 0) break;
		if (Occur[i][j] == Occur[x][y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}

	i = x, j = y;

	while (true) {
		i--; j++;
		if (i < 0 || j > 7) break;
		if (Occur[i][j] == Occur[x][y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}


	i = x, j = y;

	while (true) {
		i++; j--;
		if (i > 7 || j < 0) break;
		if (Occur[i][j] == Occur[x][y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}

	i = x, j = y;

	while (true) {
		i++; j++;
		if (i > 7 || j > 7) break;
		if (Occur[i][j] == Occur[x][y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}

	for (int i = 0; i < 8; i++) {
		int u = x - Knx[i];
		int v = y - Kny[i];
		if (u < 0 || v < 0 || u > 7 || v > 7 || Occur[u][v] == Occur[u][v]) continue;
		if (BoardState[x][y] == knight_main + 100) return 1;
	}

	if (x < 7 && y < 7 && BoardState[x + 1][y + 1] == pawn_main + 100) return 1;
	if (x >= 1 && y < 7 && BoardState[x - 1][y + 1] == pawn_main + 100) return 1;

	return 0;
}

bool AI::KingSafe(pii temp) {
	bool ok = 1;
	int x = temp.first; 
	int y = temp.second; 

	if (x < 0 || y < 0) return 0; 

	if (Turn == 0) {
		int u = White[index].x; 
		int v = White[index].y; 

		if (u < 0 || v < 0) return 0; 

		White[index].x = temp.first;
		White[index].y = temp.second;

		UpdBoardState();

		Occur[x][y] = 2;
		if (check()) ok = 0;

		White[index].x = u; 
		White[index].y = v; 
		UpdBoardState();
		return ok;
	}

	int u = Black[index].x;
	int v = Black[index].y;

	if (u < 0 || v < 0) return 0;
	
	Black[index].x = temp.first;
	Black[index].y = temp.second;

	UpdBoardState();

	Occur[x][y] = 1;
	if (check()) ok = 0;

	Black[index].x = u; 
	Black[index].y = v;
	UpdBoardState();

	return ok;
}

void AI::Castling(vector <pii>& move) {
	if (Turn == 0) {
		int x = White[4].x;
		int y = White[4].y;

		if (White[0].played == 0) {
			int l = min(White[0].x, x);
			int r = max(White[0].x, x);
			bool ok = 1; 

			if (l < 0 || r < 0) return;

			for (int i = l + 1; i < r; i++)  if (Occur[i][y] != 0) {
				ok = 0; break;
			}

			if (ok && KingSafe(pii(x - 1, y)) && KingSafe(pii(x - 2, y))) {
				move.push_back(pii(x - 2, y));
			}
		}

		if (White[7].played == 0) {
			int l = min(White[7].x, x);
			int r = max(White[7].x, x);

			if (l < 0 || r < 0) return;

			for (int i = l + 1; i < r; i++)  if (Occur[i][y] != 0) {
				return;
			}

			if (KingSafe(pii(x + 1, y)) && KingSafe(pii(x + 2, y))) {
				move.push_back(pii(x + 2, y));
			}
		}

		return;
	}

	int x = Black[4].x;
	int y = Black[4].y;


	if (Black[0].played == 0) {
		int l = min(Black[0].x, x);
		int r = max(Black[0].x, x);

		bool ok = 1;
		if (l < 0 || r < 0) return;

		for (int i = l + 1; i < r; i++)  if (Occur[i][y] != 0) {
			ok = 0; break;
		}

		if (ok && KingSafe(pii(x - 1, y)) && KingSafe(pii(x - 2, y))) {
			move.push_back(pii(x - 2, y));
		}
	}

	if (Black[7].played == 0) {
		int l = min(Black[7].x, x);
		int r = max(Black[7].x, x);

		if (l < 0 || r < 0) return;

		for (int i = l + 1; i < r; i++)  if (Occur[i][y] != 0) return;

		if (KingSafe(pii(x + 1, y)) && KingSafe(pii(x + 2, y))) {
			move.push_back(pii(x + 2, y));
		}
	}
}

void AI::UpdHistoryWhite(Pieces A, int x, int y) {
	array<int, 4> lastMove;

	lastMove[0] = x;
	lastMove[1] = y;
	lastMove[2] = index;
	lastMove[3] = A.type;

	White_history.push_back(lastMove);
}

void AI::UpdHistoryBlack(Pieces A, int x, int y) {
	array<int, 4> lastMove;

	lastMove[0] = x; 
	lastMove[1] = y;
	lastMove[2] = index;
	lastMove[3] = A.type;

	Black_history.push_back(lastMove);
}
