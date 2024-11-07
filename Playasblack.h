#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include<cstring>
#include <array>
#include <vector>
#include "Constant.h"

using namespace sf;
using namespace std;


class Playasblack {
public:
    Playasblack();
    void BoardSetUp(string path, char numberBoard);
    void draw(sf::RenderWindow& window);
    Vector2i getCord(Figure A);
    int handleEvents(sf::RenderWindow& window);

    void drawValidMoves(RenderWindow& window, const std::vector<Vector2i>& validmoves);
    bool KingSafe(Vector2i temp);
    void getValidMove(Figure A, vector<Vector2i>& move);
    void UpdBoardState();
    void PawnLogic(Figure A, vector<Vector2i>& move);
    bool isValidMove(Figure A, Vector2i pos);
    void UpdHistoryWhite(Figure A, Vector2i pos);
    void UpdHistoryBlack(Figure A, Vector2i pos);
    void KingLogic(Figure A, vector <Vector2i>& move);
    void KnightLogic(Figure A, vector<Vector2i>& move);
    void Horizontal(Figure A, vector<Vector2i>& move);
    void Vertical(Figure A, vector<Vector2i>& move);
    void Diagonal(Figure A, vector<Vector2i>& move);
    bool TakePiece(Vector2i pos);
    void Castling(vector <Vector2i>& move);
    bool WarningCheck(Figure A, Vector2i pos);

    void drawPawnPromote(RenderWindow& window);
    int PawnPromotion(Event event, RenderWindow& window);
    void drawUndoRedo(RenderWindow& window);
    void UndoRedo(Event event, RenderWindow& window);
    void ResetRedo();
    void ResetAll();
    int EndGame();
    Vector2i getAImove();
    void GetDiffculty(bool _easy, bool _medium, bool _hard);
    bool check();
    void UseRedo();
    void UseUndo();
private:
    bool isWhiteTurn, pawnPromote, pieceSelected, Easy, Medium, Hard;
    int index;
    int isGameEnd;
    vector <Vector2i> validMoves;
    Font font;
    RectangleShape Back;
    RectangleShape Save_Game;
    RectangleShape playAgain, Quit;

    Texture BoardTexture, Test;
    Sprite Board, t;
    Text cancelText;

    Texture black[6];
    Texture white[6];

    Figure Black[16];
    Figure White[16];
    Vector2i target;
    Sprite sample[5], Undo, Redo, Reset;

    array<int, 4> SaveBoard[1001][33];
    array<int, 4> store[1001][33];

    vector <array<int, 4>> Black_history, White_history;

    int BoardState[10][10];
    int Occur[10][10];
    int NumberOfMove;
    int NumberOfUndo;

    SoundBuffer sBuffer[5];
    Sound sound[5];
};