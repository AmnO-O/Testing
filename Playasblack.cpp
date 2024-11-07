#include "Playasblack.h"
#include "Constant.h"

vector<string> musicccc = {
	"./Assets/Sounds/Default/move-self.wav",
	"./Assets/Sounds/Default/capture.wav",
	"./Assets/Sounds/Default/move-check.wav",
	"./Assets/Sounds/Default/castle.wav",
	"./Assets/Sounds/Default/promote.wav"
};

Playasblack::Playasblack() {
	font.loadFromFile("Fonts/Arial.ttf");

	for (int i = 0; i <= 4; i++) {
		sBuffer[i].loadFromFile(musicccc[i]);
		sound[i].setBuffer(sBuffer[i]);
	}

	index = -1;
	pawnPromote = pieceSelected = NumberOfMove = NumberOfUndo = 0;
	isWhiteTurn = 1; isGameEnd = 0;
	validMoves.clear();
	Black_history.clear();
	White_history.clear();
	playAgain.setSize(Vector2f(200, 50));
	playAgain.setFillColor(sf::Color::Green);
	playAgain.setPosition(300, 220);
	Save_Game.setSize(sf::Vector2f(200, 50));
	Save_Game.setFillColor(sf::Color::Cyan);
	Save_Game.setPosition(300, 360);
	Quit.setSize(sf::Vector2f(200, 50));
	Quit.setFillColor(sf::Color::Red);
	Quit.setPosition(300, 360);
}

Vector2i Playasblack::getCord(Figure A) {
	return Vector2i((int)A.shape.getPosition().x / SIZE, (int)A.shape.getPosition().y / SIZE);
}

void Playasblack::PawnLogic(Figure A, vector<Vector2i>& move) {
	Vector2i cord = getCord(A);

	if (isWhiteTurn) {
		if (cord.y > 0 && BoardState[cord.x][cord.y + 1] == 0) {
			move.push_back(Vector2i(cord.x, cord.y + 1));
		}

		if (A.played == 0 && BoardState[cord.x][cord.y + 2] == 0 && BoardState[cord.x][cord.y + 1] == 0) {
			move.push_back(Vector2i(cord.x, cord.y + 2));
		}

		if (cord.y < 7 && cord.x < 7 && Occur[cord.x][cord.y] != Occur[cord.x + 1][cord.y + 1] && Occur[cord.x + 1][cord.y + 1] > 0) {
			move.push_back(Vector2i(cord.x + 1, cord.y + 1));
		}

		if (cord.y < 7 && cord.x > 0 && Occur[cord.x][cord.y] != Occur[cord.x - 1][cord.y + 1] && Occur[cord.x - 1][cord.y + 1] > 0) {
			move.push_back(Vector2i(cord.x - 1, cord.y + 1));
		}

		if (cord.x < 7 && cord.y < 7) {
			if (Black_history.size() && Black_history.back()[3] == pawn_main && Black[Black_history.back()[2]].played == 1) {
				int lst_x = Black_history.back()[0];
				int lst_y = Black_history.back()[1];

				if (lst_x == cord.x + 1 && lst_y == cord.y) move.push_back(Vector2i(cord.x + 1, cord.y + 1));
			}
		}

		if (cord.x > 0 && cord.y < 7) {
			if (Black_history.size() && Black_history.back()[3] == pawn_main && Black[Black_history.back()[2]].played == 1) {
				int lst_x = Black_history.back()[0];
				int lst_y = Black_history.back()[1];

				if (lst_x == cord.x - 1 && lst_y == cord.y) move.push_back(Vector2i(cord.x - 1, cord.y + 1));
			}
		}


		return;
	}

	if (cord.y > 0 && Occur[cord.x][cord.y - 1] == 0) {
		move.push_back(Vector2i(cord.x, cord.y - 1));
	}

	if (A.played == 0 && Occur[cord.x][cord.y - 2] == 0 && Occur[cord.x][cord.y - 1] == 0) {
		move.push_back(Vector2i(cord.x, cord.y - 2));
	}

	if (cord.y > 0 && cord.x < 7 && Occur[cord.x][cord.y] != Occur[cord.x + 1][cord.y - 1] && Occur[cord.x + 1][cord.y - 1] > 0) {
		move.push_back(Vector2i(cord.x + 1, cord.y - 1));
	}

	if (cord.y > 0 && cord.x > 0 && Occur[cord.x][cord.y] != Occur[cord.x - 1][cord.y - 1] && Occur[cord.x - 1][cord.y - 1] > 0) {
		move.push_back(Vector2i(cord.x - 1, cord.y - 1));
	}

	if (cord.x < 7 && cord.y > 0) {
		if (White_history.size() && White_history.back()[3] == pawn_main && White[White_history.back()[2]].played == 1) {
			int lst_x = White_history.back()[0];
			int lst_y = White_history.back()[1];

			if (lst_x == cord.x + 1 && lst_y == cord.y) move.push_back(Vector2i(cord.x + 1, cord.y - 1));
		}
	}

	if (cord.x > 0 && cord.y > 0) {
		if (White_history.size() && White_history.back()[3] == pawn_main && White[White_history.back()[2]].played == 1) {
			int lst_x = White_history.back()[0];
			int lst_y = White_history.back()[1];

			if (lst_x == cord.x - 1 && lst_y == cord.y) move.push_back(Vector2i(cord.x - 1, cord.y - 1));
		}
	}
}

void Playasblack::KingLogic(Figure A, vector <Vector2i>& move) {
	Vector2i cord = getCord(A);

	for (int i = 0; i < 8; i++) {
		int x = cord.x + Kx[i];
		int y = cord.y + Ky[i];
		if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[cord.x][cord.y]) continue;
		move.push_back(Vector2i(x, y));
	}
}

void Playasblack::KnightLogic(Figure A, vector<Vector2i>& move) {
	Vector2i cord = getCord(A);

	for (int i = 0; i < 8; i++) {
		int x = cord.x + Knx[i];
		int y = cord.y + Kny[i];
		if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[cord.x][cord.y]) continue;
		move.push_back(Vector2i(x, y));
	}
}

void Playasblack::Horizontal(Figure A, vector<Vector2i>& move) {
	Vector2i cord = getCord(A);
	for (int i = cord.x + 1; i < 8; i++) {
		if (Occur[cord.x][cord.y] == Occur[i][cord.y]) break;
		move.push_back(Vector2i(i, cord.y));
		if (Occur[i][cord.y] > 0) break;
	}
	for (int i = cord.x - 1; i >= 0; i--) {
		if (Occur[cord.x][cord.y] == Occur[i][cord.y]) break;
		move.push_back(Vector2i(i, cord.y));
		if (Occur[i][cord.y] > 0) break;
	}
}

void Playasblack::Vertical(Figure A, vector<Vector2i>& move) {
	Vector2i cord = getCord(A);
	if (cord.x < 0 || cord.y < 0) return;
	for (int i = cord.y + 1; i < 8; i++) {
		if (Occur[cord.x][cord.y] == Occur[cord.x][i]) break;
		move.push_back(Vector2i(cord.x, i));
		if (Occur[cord.x][i] > 0) break;
	}
	for (int i = cord.y - 1; i >= 0; i--) {
		if (Occur[cord.x][cord.y] == Occur[cord.x][i]) break;
		move.push_back(Vector2i(cord.x, i));
		if (Occur[cord.x][i] > 0) break;
	}
}

void Playasblack::Diagonal(Figure A, vector<Vector2i>& move) {
	Vector2i cord = getCord(A);
	if (cord.x < 0 || cord.y < 0) return;

	int i, j; i = cord.x, j = cord.y;

	while (true) {
		i--; j--;
		if (i < 0 || j < 0) break;
		if (Occur[i][j] != Occur[cord.x][cord.y]) {
			move.push_back(Vector2i(i, j));
		}

		if (Occur[i][j] > 0) break;
	}

	i = cord.x, j = cord.y;

	while (true) {
		i--; j++;
		if (i < 0 || j > 7) break;
		if (Occur[i][j] != Occur[cord.x][cord.y]) {
			move.push_back(Vector2i(i, j));
		}
		if (Occur[i][j] > 0) break;
	}


	i = cord.x, j = cord.y;

	while (true) {
		i++; j--;
		if (i > 7 || j < 0) break;
		if (Occur[i][j] != Occur[cord.x][cord.y]) {
			move.push_back(Vector2i(i, j));
		}
		if (Occur[i][j] > 0) break;
	}

	i = cord.x, j = cord.y;

	while (true) {
		i++; j++;
		if (i > 7 || j > 7) break;
		if (Occur[i][j] != Occur[cord.x][cord.y]) {
			move.push_back(Vector2i(i, j));
		}
		if (Occur[i][j] > 0) break;
	}
}

void Playasblack::drawUndoRedo(RenderWindow& window) {
	Texture redo, undo, reset;

	redo.loadFromFile("images/redo.png");
	undo.loadFromFile("images/undo.png");
	reset.loadFromFile("images/reset.png");

	Redo.setTexture(redo);
	Undo.setTexture(undo);
	Reset.setTexture(reset);

	float targetSize = 60.0f;
	float scaleX = targetSize / undo.getSize().x;
	float scaleY = targetSize / undo.getSize().y;
	Undo.setScale(scaleX, scaleY);


	scaleX = targetSize / redo.getSize().x;
	scaleY = targetSize / redo.getSize().y;
	Redo.setScale(scaleX, scaleY);

	scaleX = targetSize / reset.getSize().x;
	scaleY = targetSize / reset.getSize().y;
	Reset.setScale(scaleX, scaleY);


	Undo.setPosition(629, 510);
	Redo.setPosition(712, 510);
	Reset.setPosition(670, 450);

	window.draw(Reset);
	window.draw(Undo);
	window.draw(Redo);
}

bool Playasblack::isValidMove(Figure A, Vector2i pos) {
	vector<Vector2i> move;
	getValidMove(A, move);

	for (Vector2i x : move) if (x == pos && KingSafe(x)) return 1;
	vector<Vector2i> a;

	if (A.type == king_main && A.played == 0 && KingSafe(getCord(A))) {
		Castling(a);
	}

	for (auto move : a) {
		if (pos == move) return 1;
	}
	return 0;
}

void Playasblack::getValidMove(Figure A, vector<Vector2i>& move) {
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

bool Playasblack::TakePiece(Vector2i pos) {
	if (isWhiteTurn && Black_history.size() && Black_history.back()[3] == pawn_main && White[index].type == pawn_main) {
		int lst_x = Black_history.back()[0];
		int lst_y = Black_history.back()[1];
		int id = Black_history.back()[2];

		if (Black[id].played == 1 && lst_x == pos.x && lst_y == pos.y - 1) {
			Black[id].shape.setPosition(-400, -400);
			return 1;
		}
	}
	else if (isWhiteTurn == 0 && White_history.size() && White_history.back()[3] == pawn_main && Black[index].type == pawn_main) {
		int lst_x = White_history.back()[0];
		int lst_y = White_history.back()[1];
		int id = White_history.back()[2];

		if (White[id].played == 1 && lst_x == pos.x && lst_y == pos.y + 1) {
			White[id].shape.setPosition(-400, -400);
			return 1;
		}
	}


	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(White[i]);
		if (cord != pos) continue;
		White[i].SetPosition(-500, -500);
		return 1;
	}

	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(Black[i]);
		if (cord != pos) continue;
		Black[i].SetPosition(-500, -500);
		return 1;
	}

	return 0;
}

void Playasblack::UpdBoardState() {

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			BoardState[i][j] = 0;
			Occur[i][j] = 0;
		}
	}

	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(White[i]);
		SaveBoard[NumberOfMove][i] = { cord.x, cord.y, White[i].type, White[i].played };

		if (cord.x < 0 || cord.y < 0) continue;
		BoardState[cord.x][cord.y] = White[i].type + 100;
		Occur[cord.x][cord.y] = 2;
	}

	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(Black[i]);
		SaveBoard[NumberOfMove][i + 16] = { cord.x, cord.y, Black[i].type, Black[i].played };

		if (cord.x < 0 || cord.y < 0) continue;
		BoardState[cord.x][cord.y] = Black[i].type + 200;
		Occur[cord.x][cord.y] = 1;
	}
}

void Playasblack::ResetRedo() {
	if (NumberOfUndo) {
		for (int i = 0; i < 32; i++) {
			if (SaveBoard[NumberOfMove][i] != store[NumberOfUndo][i]) {
				NumberOfUndo = 0;
				return;
			}
		}
		NumberOfUndo--;
	}
}

bool Playasblack::check() {
	if (isWhiteTurn) {
		Vector2i cord = getCord(White[4]);
		if (cord.x < 0 || cord.y < 0) return 1;

		for (int i = cord.x - 1; i >= 0; i--) {
			if (Occur[i][cord.y] == 0) continue;
			if (Occur[i][cord.y] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][cord.y] == queen_main + 200 || BoardState[i][cord.y] == rook_main + 200) return 1;
			if (Occur[i][cord.y] > 0) break;
		}

		for (int i = cord.x + 1; i <= 7; i++) {
			if (Occur[i][cord.y] == 0) continue;
			if (Occur[i][cord.y] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][cord.y] == queen_main + 200 || BoardState[i][cord.y] == rook_main + 200) return 1;
			if (Occur[i][cord.y] > 0) break;
		}

		for (int i = cord.y + 1; i <= 7; i++) {
			if (Occur[cord.x][i] == 0) continue;
			if (Occur[cord.x][i] == Occur[cord.x][cord.y]) break;
			if (BoardState[cord.x][i] == queen_main + 200 || BoardState[cord.x][i] == rook_main + 200) return 1;
			if (Occur[cord.x][i] > 0) break;
		}

		for (int i = cord.y - 1; i >= 0; i--) {
			if (Occur[cord.x][i] == 0) continue;
			if (Occur[cord.x][i] == Occur[cord.x][cord.y]) break;
			if (BoardState[cord.x][i] == queen_main + 200 || BoardState[cord.x][i] == rook_main + 200) return 1;
			if (Occur[cord.x][i] > 0) break;
		}


		int i, j; i = cord.x, j = cord.y;

		while (true) {
			i--; j--;
			if (i < 0 || j < 0) break;
			if (Occur[i][j] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}

		i = cord.x, j = cord.y;

		while (true) {
			i--; j++;
			if (i < 0 || j > 7) break;
			if (Occur[i][j] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}


		i = cord.x, j = cord.y;

		while (true) {
			i++; j--;
			if (i > 7 || j < 0) break;
			if (Occur[i][j] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}

		i = cord.x, j = cord.y;

		while (true) {
			i++; j++;
			if (i > 7 || j > 7) break;
			if (Occur[i][j] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}


		for (int i = 0; i < 8; i++) {
			int x = cord.x - Knx[i];
			int y = cord.y - Kny[i];
			if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[cord.x][cord.y]) continue;
			if (BoardState[x][y] == knight_main + 200) return 1;
		}

		
		if (cord.x < 7 && cord.y < 7 && BoardState[cord.x + 1][cord.y + 1] == pawn_main + 200) return 1;
		if (cord.x >= 1 && cord.y < 7 && BoardState[cord.x - 1][cord.y + 1] == pawn_main + 200) return 1;

		return 0;
	}

	Vector2i cord = getCord(Black[4]);
	if (cord.x < 0 || cord.y < 0) return 1;

	for (int i = cord.x - 1; i >= 0; i--) {
		if (Occur[i][cord.y] == 0) continue;
		if (Occur[i][cord.y] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][cord.y] == queen_main + 100 || BoardState[i][cord.y] == rook_main + 100) return 1;
		if (Occur[i][cord.y] > 0) break;
	}

	for (int i = cord.x + 1; i <= 7; i++) {
		if (Occur[i][cord.y] == 0) continue;
		if (Occur[i][cord.y] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][cord.y] == queen_main + 100 || BoardState[i][cord.y] == rook_main + 100) return 1;
		if (Occur[i][cord.y] > 0) break;
	}

	for (int i = cord.y + 1; i <= 7; i++) {
		if (Occur[cord.x][i] == 0) continue;
		if (Occur[cord.x][i] == Occur[cord.x][cord.y]) break;
		if (BoardState[cord.x][i] == queen_main + 100 || BoardState[cord.x][i] == rook_main + 100) return 1;
		if (Occur[cord.x][i] > 0) break;
	}

	for (int i = cord.y - 1; i >= 0; i--) {
		if (Occur[cord.x][i] == 0) continue;
		if (Occur[cord.x][i] == Occur[cord.x][cord.y]) break;
		if (BoardState[cord.x][i] == queen_main + 100 || BoardState[cord.x][i] == rook_main + 100) return 1;
		if (Occur[cord.x][i] > 0) break;
	}


	int i, j; i = cord.x, j = cord.y;

	while (true) {
		i--; j--;
		if (i < 0 || j < 0) break;
		if (Occur[i][j] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}

	i = cord.x, j = cord.y;

	while (true) {
		i--; j++;
		if (i < 0 || j > 7) break;
		if (Occur[i][j] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}


	i = cord.x, j = cord.y;

	while (true) {
		i++; j--;
		if (i > 7 || j < 0) break;
		if (Occur[i][j] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}

	i = cord.x, j = cord.y;

	while (true) {
		i++; j++;
		if (i > 7 || j > 7) break;
		if (Occur[i][j] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}

	for (int i = 0; i < 8; i++) {
		int x = cord.x - Knx[i];
		int y = cord.y - Kny[i];
		if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[cord.x][cord.y]) continue;
		if (BoardState[x][y] == knight_main + 100) return 1;
	}

	if (cord.x >= 1 && cord.y >= 1 && BoardState[cord.x - 1][cord.y - 1] == pawn_main + 100) return 1;
	if (cord.x < 7 && cord.y >= 1 && BoardState[cord.x + 1][cord.y - 1] == pawn_main + 100) return 1;

	return 0;
}

bool Playasblack::KingSafe(Vector2i temp) {
	bool ok = 1;

	if (isWhiteTurn) {
		Vector2f real_position = White[index].shape.getPosition();
		White[index].shape.setPosition(temp.x * SIZE, temp.y * SIZE);
		UpdBoardState();

		Occur[temp.x][temp.y] = 2;
		if (check()) ok = 0;

		White[index].shape.setPosition(real_position);
		UpdBoardState();
		return ok;
	}

	Vector2f real_position = Black[index].shape.getPosition();
	Black[index].shape.setPosition(temp.x * SIZE, temp.y * SIZE);
	UpdBoardState();

	Occur[temp.x][temp.y] = 1;
	if (check()) ok = 0;

	Black[index].shape.setPosition(real_position);
	UpdBoardState();

	return ok;
}

void Playasblack::Castling(vector <Vector2i>& move) {
	if (isWhiteTurn) {
		Vector2i king = getCord(White[4]);

		if (White[0].played == 0) {
			Vector2i rook = getCord(White[0]);
			int l = rook.x;
			int r = king.x;
			swap(l, r); 
			bool ok = 1;
			if (rook.x < 0 || rook.y < 0) ok = 0;

			for (int i = l + 1; i < r; i++)  if (Occur[i][king.y] != 0) {
				ok = 0; break;
			}

			if (ok && KingSafe(Vector2i(king.x + 1, king.y)) && KingSafe(Vector2i(king.x + 2, king.y))) {
				move.push_back(Vector2i(king.x + 2, king.y));
			}
		}

		if (White[7].played == 0) {
			Vector2i rook = getCord(White[7]);
			int l = king.x;
			int r = rook.x;
			swap(l, r);
			if (rook.x < 0 || rook.y < 0) return;

			for (int i = l + 1; i < r; i++)  if (Occur[i][king.y] != 0) {
				return;
			}

			if (KingSafe(Vector2i(king.x - 1, king.y)) && KingSafe(Vector2i(king.x - 2, king.y))) {
				move.push_back(Vector2i(king.x - 2, king.y));
			}
		}

		return;
	}

	Vector2i king = getCord(Black[4]);

	if (Black[0].played == 0) {
		Vector2i rook = getCord(Black[0]);
		int l = rook.x;
		int r = king.x;
		swap(l, r);
		bool ok = 1;
		if (rook.x < 0 || rook.y < 0) ok = 0;

		for (int i = l + 1; i < r; i++)  if (Occur[i][king.y] != 0) {
			ok = 0; break;
		}

		if (ok && KingSafe(Vector2i(king.x + 1, king.y)) && KingSafe(Vector2i(king.x + 2, king.y))) {
			move.push_back(Vector2i(king.x + 2, king.y));
		}
	}

	if (Black[7].played == 0) {
		Vector2i rook = getCord(Black[7]);
		int l = king.x;
		int r = rook.x;
		if (rook.x < 0 || rook.y < 0) return;
		swap(l, r);
		for (int i = l + 1; i < r; i++)  if (Occur[i][king.y] != 0) return;

		if (KingSafe(Vector2i(king.x - 1, king.y)) && KingSafe(Vector2i(king.x - 2, king.y))) {
			move.push_back(Vector2i(king.x - 2, king.y));
		}
	}
}

void Playasblack::drawValidMoves(RenderWindow& window, const std::vector<Vector2i>& validmoves) {
	int TILE_SIZE = SIZE;
	int TILE_OFFSET = 24;

	CircleShape highlight(13);
	highlight.setFillColor(Color(0, 255, 0, 100));


	for (const auto& move : validmoves) if (KingSafe(move)) {
		highlight.setPosition(move.x * TILE_SIZE + TILE_OFFSET, move.y * TILE_SIZE + TILE_OFFSET);
		window.draw(highlight);
	}

	vector<Vector2i> a;
	if (isWhiteTurn && White[index].type == king_main && White[index].played == 0 && KingSafe(getCord(White[index]))) {
		Castling(a);
	}
	else if (isWhiteTurn == 0 && Black[index].type == king_main && Black[index].played == 0 && KingSafe(getCord(Black[index]))) {
		Castling(a);
	}

	for (auto move : a) {
		highlight.setPosition(move.x * TILE_SIZE + TILE_OFFSET, move.y * TILE_SIZE + TILE_OFFSET);
		window.draw(highlight);
	}
}

void Playasblack::UpdHistoryWhite(Figure A, Vector2i pos) {
	Vector2f cord = A.shape.getPosition();
	array<int, 4> lastMove;

	lastMove[0] = pos.x;
	lastMove[1] = pos.y;
	lastMove[2] = index;
	lastMove[3] = A.type;

	White_history.push_back(lastMove);
}

void Playasblack::UpdHistoryBlack(Figure A, Vector2i pos) {
	Vector2f cord = A.shape.getPosition();
	array<int, 4> lastMove;

	lastMove[0] = pos.x;
	lastMove[1] = pos.y;
	lastMove[2] = index;
	lastMove[3] = A.type;

	Black_history.push_back(lastMove);
}

bool Playasblack::WarningCheck(Figure A, Vector2i pos) {
	if (A.type == king_main && isWhiteTurn && A.played == 0) {
		Vector2i cord = getCord(A);

		if (pos.x == cord.x - 2 && pos.y == cord.y) {

			A = White[0];
			pos.x++;
		}
		else if (pos.x == cord.x + 2 && pos.y == cord.y) {
			A = White[7];
			pos.x--;
		}
		else return 0;
	}
	else if (A.type == king_main && isWhiteTurn == 0 && A.played == 0) {
		Vector2i cord = getCord(A);

		if (pos.x == cord.x - 2 && pos.y == cord.y) {
			A = Black[0];
			pos.x++;
		}
		else if (pos.x == cord.x + 2 && pos.y == cord.y) {
			A = Black[7];
			pos.x--;
		}
		else return 0;
	}

	Vector2i cord = getCord(A);


	Occur[cord.x][cord.y] = 0;
	int prv = Occur[pos.x][pos.y];
	Occur[pos.x][pos.y] = (isWhiteTurn ? 2 : 1);

	vector<Vector2i> a;
	A.shape.setPosition(pos.x * SIZE, pos.y * SIZE);

	getValidMove(A, a);

	for (auto move : a) {
		if (BoardState[move.x][move.y] == king_main + 100 || BoardState[move.x][move.y] == king_main + 200) {
			if (Occur[pos.x][pos.y] == Occur[move.x][move.y] || min(Occur[pos.x][pos.y], Occur[move.x][move.y]) == 0) continue;
			Occur[cord.x][cord.y] = (isWhiteTurn ? 2 : 1);
			Occur[pos.x][pos.y] = prv;
			return 1;
		}
	}

	Occur[pos.x][pos.y] = prv;
	Occur[cord.x][cord.y] = (isWhiteTurn ? 2 : 1);

	return 0;
}

void Playasblack::drawPawnPromote(RenderWindow& window) {
	float promotionBoardX = 700.0f;
	float promotionBoardY = 45.0f;
	float iconSize = 60.0f;
	sf::RectangleShape background(sf::Vector2f(iconSize, iconSize * 5 + 20));
	background.setPosition(promotionBoardX, promotionBoardY);
	background.setFillColor(sf::Color(240, 240, 240));
	window.draw(background);

	if (isWhiteTurn) {
		Vector2i cord = getCord(White[index]);

		for (int i = 1; i <= 4; i++) {
			sample[i].setTexture(white[i]);
			sample[i].setScale(iconSize / sample[i].getLocalBounds().width, iconSize / sample[i].getLocalBounds().height);
			sample[i].setPosition(promotionBoardX, promotionBoardY + (i - 1) * iconSize);
			window.draw(sample[i]);
		}

	}
	else {
		Vector2i cord = getCord(White[index]);

		for (int i = 1; i <= 4; ++i) {
			sample[i].setTexture(black[i]);
			sample[i].setScale(iconSize / sample[i].getLocalBounds().width, iconSize / sample[i].getLocalBounds().height);
			sample[i].setPosition(promotionBoardX, promotionBoardY + (i - 1) * iconSize);
			window.draw(sample[i]);
		}
	}

	cancelText.setFont(font);
	cancelText.setString("X");
	cancelText.setCharacterSize(30);
	cancelText.setFillColor(sf::Color(150, 150, 150));
	cancelText.setPosition(promotionBoardX + 20, promotionBoardY + 4 * iconSize + 10);
	window.draw(cancelText);
}

int Playasblack::PawnPromotion(Event event, RenderWindow& window) {
	Vector2i pos = Mouse::getPosition(window);
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
		for (int i = 1; i <= 4; i++) {
			if (sample[i].getGlobalBounds().contains((float)pos.x, (float)pos.y)) {
				pawnPromote = 0;
				return i;
			}
		}
	}

	if (cancelText.getGlobalBounds().contains(pos.x, pos.y)) pawnPromote = 0;

	return 0;
}

void Playasblack::UseUndo() {
	NumberOfUndo++;

	for (int i = 0; i < 32; i++)
		store[NumberOfUndo][i] = SaveBoard[NumberOfMove][i];

	NumberOfMove--;
	isWhiteTurn = 1;

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	UpdBoardState();

	NumberOfUndo++;

	for (int i = 0; i < 32; i++)
		store[NumberOfUndo][i] = SaveBoard[NumberOfMove][i];

	NumberOfMove--;
	isWhiteTurn = 0;

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	UpdBoardState();
}

void Playasblack::UseRedo() {
	NumberOfMove++;

	for (int i = 0; i < 32; i++)
		SaveBoard[NumberOfMove][i] = store[NumberOfUndo][i];

	NumberOfUndo--;
	isWhiteTurn = 1;

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	UpdBoardState();

	NumberOfMove++;

	for (int i = 0; i < 32; i++)
		SaveBoard[NumberOfMove][i] = store[NumberOfUndo][i];

	NumberOfUndo--;
	isWhiteTurn = 0;

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	UpdBoardState();
}

void Playasblack::UndoRedo(Event event, RenderWindow& window) {
	Vector2i pos = Mouse::getPosition(window);

	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Undo.getGlobalBounds().contains(pos.x, pos.y) && NumberOfMove > 1) {
		UseUndo();
	}

	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Redo.getGlobalBounds().contains(pos.x, pos.y) && NumberOfUndo > 1) {
		UseRedo();
	}

	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Left && NumberOfMove > 1) {
			UseUndo();
		}

		if (event.key.code == sf::Keyboard::Right && NumberOfUndo > 1) {
			UseRedo();
		}
	}
}
void Playasblack::ResetAll() {
	index = -1;
	pawnPromote = pieceSelected = NumberOfMove = NumberOfUndo = 0;
	isWhiteTurn = 1; isGameEnd = 0;
	validMoves.clear();
	Black_history.clear();
	White_history.clear();

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	UpdBoardState();
}

int Playasblack::EndGame() {
	if (isWhiteTurn) {
		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(White[i]);
			if (cord.x < 0 || cord.y < 0) continue;
			vector<Vector2i> valid;
			getValidMove(White[i], valid);

			for (auto move : valid) {
				index = i;
				if (KingSafe(move)) return 0;
			}
		}
		if (check()) return 1;
		return 2;
	}

	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(Black[i]);
		if (cord.x < 0 || cord.y < 0) continue;
		vector<Vector2i> valid;
		getValidMove(Black[i], valid);

		for (auto move : valid) {
			index = i;
			if (KingSafe(move)) return 0;
		}
	}
	if (check()) return 1;
	return 2; /// draw
}

void Playasblack::GetDiffculty(bool _easy, bool _medium, bool _hard) {
	Easy = _easy; Medium = _medium; Hard = _hard;
}

Vector2i Playasblack::getAImove() {
	Vector2i valid;

	if (Easy) {
		vector<int>indicies;
		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(White[i]);
			if (cord.x < 0 || cord.y < 0) continue;
			indicies.push_back(i);
		}

		shuffle(indicies.begin(), indicies.end(), rd);

		while (indicies.size()) {
			vector<Vector2i> valid;
			getValidMove(White[indicies.back()], valid);
			index = indicies.back();

			for (auto move : valid) if (KingSafe(move)) {
				return move;
			}

			indicies.pop_back();
		}
	}

	if (Medium) {

	}

	if (Hard) {

	}

	return Vector2i(0, 1);
}

int Playasblack::handleEvents(RenderWindow& window) {
	Event event;

	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) window.close();
		Vector2i pos = Mouse::getPosition(window);
		UndoRedo(event, window);

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Reset.getGlobalBounds().contains(pos.x, pos.y)) {
			ResetAll();
			continue;
		}

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Quit.getGlobalBounds().contains(pos.x, pos.y)) {
			return Initial_main;
		}

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Save_Game.getGlobalBounds().contains(pos.x, pos.y)) {

		}


		if (isGameEnd) {
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && playAgain.getGlobalBounds().contains(pos.x, pos.y)) ResetAll();
			return Playasblack_main;
		}

		if (isWhiteTurn) {
			Vector2i AI = getAImove();

			if (isValidMove(White[index], AI)) {
				if (White[index].type == pawn_main && AI.y == 0) {
					int x = random(1, 4);
					sound[4].play();

					if (x == rook_main) White[index].shape.setTexture(white[rook_main]), White[index].type = rook_main;
					if (x == queen_main) White[index].shape.setTexture(white[queen_main]), White[index].type = queen_main;
					if (x == bishop_main) White[index].shape.setTexture(white[bishop_main]), White[index].type = bishop_main;
					if (x == knight_main) White[index].shape.setTexture(white[knight_main]), White[index].type = knight_main;

					if (WarningCheck(White[index], AI)) sound[2].play();
					TakePiece(AI);

					UpdHistoryWhite(White[index], AI);
					White[index].shape.setPosition(AI.x * SIZE, target.y * SIZE);
					White[index].played++;

					NumberOfMove++;
					UpdBoardState();
					ResetRedo();
					pieceSelected = 0;
					isWhiteTurn = 0;
					isGameEnd = EndGame();
					continue;
				}

				Vector2i cord = getCord(White[index]);

				if (WarningCheck(White[index], AI)) sound[2].play();
				else if (TakePiece(AI)) sound[1].play();
				else if (White[index].played == 0 && index == 4) {
					if (AI.x == cord.x + 2 && AI.y == cord.y) White[0].shape.setPosition((AI.x - 1) * SIZE, AI.y * SIZE), sound[3].play();
					else if (AI.x == cord.x - 2 && AI.y == cord.y) White[7].shape.setPosition((AI.x + 1) * SIZE, AI.y * SIZE), sound[3].play();
					else sound[0].play();
				}
				else sound[0].play();

				if (White[index].played == 0 && index == 4) {
					if (AI.x == cord.x + 2 && AI.y == cord.y) White[0].shape.setPosition((AI.x - 1) * SIZE, AI.y * SIZE), White[7].played++;
					else if (AI.x == cord.x - 2 && AI.y == cord.y) White[7].shape.setPosition((AI.x + 1) * SIZE, AI.y * SIZE), White[0].played++;
				}

				TakePiece(AI);

				for (int i = 0; i < 12; i++) {
					Vector2f p = Vector2f(AI.x * SIZE + 0.5, AI.y * SIZE + 0.5) - Vector2f(White[index].shape.getPosition());

					White[index].shape.move(p.x / 12, p.y / 12);
					window.draw(Board);

					for (int j = 0; j < 16; j++) window.draw(White[j].shape);
					for (int j = 0; j < 16; j++) window.draw(Black[j].shape);


					window.display();
				}

				UpdHistoryWhite(White[index], AI);

				White[index].shape.setPosition(AI.x * SIZE, AI.y * SIZE);
				White[index].played++;
				NumberOfMove++;

				UpdBoardState();
				ResetRedo();
				isWhiteTurn = 0;
				isGameEnd = EndGame();
			}

			continue;
		}


		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
			if (pieceSelected == 0) {
				if (isWhiteTurn == 0){
					for (int i = 0; i < 16; i++) {
						if (Black[i].shape.getGlobalBounds().contains(pos.x, pos.y)) {
							index = i;
							pieceSelected = 1;
						}
					}

					if (pieceSelected) {
						validMoves.clear();
						getValidMove(Black[index], validMoves);
					}
				}
				continue; 
			}
			
			Vector2i temp = pos;
			temp.x = temp.x / SIZE;
			temp.y = temp.y / SIZE;

			if (pawnPromote) {
				int x = PawnPromotion(event, window);
				if (x) sound[4].play();
				else continue;

				if (x == rook_main) Black[index].shape.setTexture(black[rook_main]), Black[index].type = rook_main;
				if (x == queen_main) Black[index].shape.setTexture(black[queen_main]), Black[index].type = queen_main;
				if (x == bishop_main) Black[index].shape.setTexture(black[bishop_main]), Black[index].type = bishop_main;
				if (x == knight_main) Black[index].shape.setTexture(black[knight_main]), Black[index].type = knight_main;

				if (WarningCheck(Black[index], target)) sound[2].play();
				TakePiece(target);

				UpdHistoryBlack(Black[index], target);
				Black[index].shape.setPosition(target.x * SIZE, target.y * SIZE);
				Black[index].played++;
				NumberOfMove++;

				UpdBoardState();
				ResetRedo();
				pieceSelected = 0;
				isWhiteTurn = 1;
				isGameEnd = EndGame();
			}

			if (isValidMove(Black[index], temp)) {
				if (Black[index].type == pawn_main && temp.y == 0) {
					pawnPromote = 1;
					target = temp;
					continue;
				}

				Vector2i cord = getCord(Black[index]);

				if (WarningCheck(Black[index], temp)) sound[2].play();
				else if (TakePiece(temp)) sound[1].play();
				else if (Black[index].played == 0 && index == 4) {
					if (temp.x == cord.x + 2 && temp.y == cord.y) Black[0].shape.setPosition((temp.x - 1) * SIZE, temp.y * SIZE), sound[3].play();
					else if (temp.x == cord.x - 2 && temp.y == cord.y) Black[7].shape.setPosition((temp.x + 1) * SIZE, temp.y * SIZE), sound[3].play();
					else sound[0].play();
				}
				else sound[0].play();

				TakePiece(temp);

				if (Black[index].played == 0 && index == 4) {
					if (temp.x == cord.x + 2 && temp.y == cord.y) Black[0].shape.setPosition((temp.x - 1) * SIZE, temp.y * SIZE), Black[0].played++;
					else if (temp.x == cord.x - 2 && temp.y == cord.y) Black[7].shape.setPosition((temp.x + 1) * SIZE, temp.y * SIZE), Black[7].played++;
				}

				for (int i = 0; i < 12; i++) {
					Vector2f p = Vector2f(temp.x * SIZE + 0.5, temp.y * SIZE + 0.5) - Vector2f(Black[index].shape.getPosition());

					Black[index].shape.move(p.x / 12, p.y / 12);
					window.draw(Board);

					for (int j = 0; j < 16; j++) window.draw(White[j].shape);
					for (int j = 0; j < 16; j++) window.draw(Black[j].shape);


					window.display();
				}

				UpdHistoryBlack(Black[index], temp);

				NumberOfMove++;
				Black[index].shape.setPosition(temp.x * SIZE, temp.y * SIZE);
				Black[index].played++;

				UpdBoardState();
				ResetRedo();
				isWhiteTurn = 1;
				isGameEnd = EndGame();
			}

			pieceSelected = 0;
		}
	}
	return Playasblack_main;
}

void Playasblack::BoardSetUp(string path, char numberBoard) {
	string BoardPath = "images/BoardGame/rboard";
	BoardPath += numberBoard;
	BoardPath += ".png";

	BoardTexture.loadFromFile(BoardPath);
	Board.setTexture(BoardTexture);

	int cnt = 0;
	string tmp = path;

	tmp += "white-rook.png";
	white[rook_main].loadFromFile(tmp);
	White[0] = Figure(rook_main, white[rook_main]);
	White[7] = Figure(rook_main, white[rook_main]);

	tmp = path; tmp += "white-knight.png";
	white[knight_main].loadFromFile(tmp);

	White[1] = Figure(knight_main, white[knight_main]);
	White[6] = Figure(knight_main, white[knight_main]);

	tmp = path; tmp += "white-bishop.png";
	white[bishop_main].loadFromFile(tmp);
	White[2] = Figure(bishop_main, white[bishop_main]);
	White[5] = Figure(bishop_main, white[bishop_main]);

	tmp = path; tmp += "white-queen.png";
	white[queen_main].loadFromFile(tmp);
	White[3] = Figure(queen_main, white[queen_main]);

	tmp = path; tmp += "white-king.png";
	white[king_main].loadFromFile(tmp);
	White[4] = Figure(king_main, white[king_main]);

	tmp = path;  tmp += "black-rook.png";
	black[rook_main].loadFromFile(tmp);
	Black[0] = Figure(rook_main, black[rook_main]);
	Black[7] = Figure(rook_main, black[rook_main]);

	tmp = path; tmp += "black-knight.png";
	black[knight_main].loadFromFile(tmp);
	Black[1] = Figure(knight_main, black[knight_main]);
	Black[6] = Figure(knight_main, black[knight_main]);

	tmp = path; tmp += "black-bishop.png";
	black[bishop_main].loadFromFile(tmp);
	Black[2] = Figure(bishop_main, black[bishop_main]);
	Black[5] = Figure(bishop_main, black[bishop_main]);

	tmp = path; tmp += "black-queen.png";
	black[queen_main].loadFromFile(tmp);
	Black[3] = Figure(queen_main, black[queen_main]);

	Test.loadFromFile(tmp);
	t.setTexture(Test);
	t.setPosition(0, SIZE);

	tmp = path; tmp += "black-king.png";
	black[king_main].loadFromFile(tmp);
	Black[4] = Figure(king_main, black[king_main]);

	for (int i = 8; i < 16; i++) {
		tmp = path; tmp += "white-pawn.png";
		white[5].loadFromFile(tmp);
		White[i] = Figure(pawn_main, white[5]);
		tmp = path; tmp += "black-pawn.png";
		black[5].loadFromFile(tmp);
		Black[i] = Figure(pawn_main, black[5]);
	}

	for (int i = 0; i < 16; i++) {
		if (i < 8) White[i].SetPosition((7 - cnt) * SIZE, 0);
		else White[i].SetPosition((7 - cnt) * SIZE, SIZE);

		if (i < 8) Black[i].SetPosition((7 - cnt) * SIZE, 7 * SIZE);
		else Black[i].SetPosition((7 - cnt) * SIZE, 6 * SIZE);

		cnt++;
		if (cnt == 8) cnt = 0;
	}

	UpdBoardState();
}

void Playasblack::draw(sf::RenderWindow& window) {
	window.draw(Board);

	for (int i = 0; i < 16; i++) {
		window.draw(Black[i].shape);
		window.draw(White[i].shape);
	}

	if (pieceSelected) {
		drawValidMoves(window, validMoves);
	}

	if (pawnPromote) {
		drawPawnPromote(window);
	}

	drawUndoRedo(window);

	sf::Text quitButtonText, SaveGameText;
	Quit.setPosition(600, 400);
	Save_Game.setPosition(600, 350);
	SaveGameText.setFont(font);
	SaveGameText.setString("Save");
	SaveGameText.setCharacterSize(30);
	SaveGameText.setFillColor(sf::Color::White);

	SaveGameText.setPosition(
		Save_Game.getPosition().x + (Save_Game.getSize().x - SaveGameText.getLocalBounds().width) / 2,
		Save_Game.getPosition().y + (Save_Game.getSize().y - SaveGameText.getLocalBounds().height) / 2 - 5
	);

	quitButtonText.setFont(font);
	quitButtonText.setString("Quit");
	quitButtonText.setCharacterSize(30);
	quitButtonText.setFillColor(sf::Color::White);
	quitButtonText.setPosition(
		Quit.getPosition().x + (Quit.getSize().x - quitButtonText.getLocalBounds().width) / 2,
		Quit.getPosition().y + (Quit.getSize().y - quitButtonText.getLocalBounds().height) / 2 - 5
	);

	window.draw(Quit);
	window.draw(quitButtonText);
	window.draw(Save_Game);
	window.draw(SaveGameText);

	if (isGameEnd) {
		Quit.setSize(sf::Vector2f(200, 50));
		Quit.setFillColor(sf::Color::Red);
		Quit.setPosition(300, 360);

		sf::Text endText;
		endText.setFont(font);

		if (isGameEnd == 1) {
			if (isWhiteTurn) endText.setString("You Win!");
			if (isWhiteTurn == 0) endText.setString("Bot Wins!");
		}
		else endText.setString("Draw\n");

		sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
		overlay.setFillColor(sf::Color(0, 0, 0, 150));

		endText.setCharacterSize(50);
		endText.setFillColor(sf::Color::White);
		endText.setPosition(window.getSize().x / 2 - endText.getLocalBounds().width / 2,
			window.getSize().y / 2 - endText.getLocalBounds().height / 2);

		sf::Text playAgainButtonText;
		playAgainButtonText.setFont(font);
		playAgainButtonText.setString("Play Again");
		playAgainButtonText.setCharacterSize(30);
		playAgainButtonText.setFillColor(sf::Color::White);

		playAgainButtonText.setPosition(
			playAgain.getPosition().x + (playAgain.getSize().x - playAgainButtonText.getLocalBounds().width) / 2,
			playAgain.getPosition().y + (playAgain.getSize().y - playAgainButtonText.getLocalBounds().height) / 2 - 5
		);

		quitButtonText.setPosition(
			Quit.getPosition().x + (Quit.getSize().x - quitButtonText.getLocalBounds().width) / 2,
			Quit.getPosition().y + (Quit.getSize().y - quitButtonText.getLocalBounds().height) / 2 - 5
		);

		window.draw(overlay);
		window.draw(endText);
		window.draw(playAgain);
		window.draw(playAgainButtonText);
		window.draw(Quit);
		window.draw(quitButtonText);
	}
}
