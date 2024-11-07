// main.cpp

#include <SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include "Menu.h"
#include "Constant.h"
#include "Mode.h"
#include "Settings.h"
#include "TwoPlayerMode.h"
#include "OnePlayerMode.h"
#include "Playaswhite.h"
#include "Playasblack.h"
#include <memory>

using namespace sf; 
int GameState = Initial_main; 

void saveGameHistory() {
    ofstream fout("move_history.txt");
    if (fout.is_open()) {

        fout.close();
    }
    else {
        cout << "Unable to open file history for writing.\n";
    }
}

void readGameHistory() {
    ifstream inFile("game_history.txt");
    if (inFile.is_open()) {

        inFile.close(); 
    }
    else {
        cout << "Unable to open history file for reading\n"; 
    }
}


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Chess Game");

    auto menu = std::make_unique<Menu>();
    auto mode = std::make_unique<Mode>();
    auto modify = std::make_unique<Settings>();
    auto twoPlayers = std::make_unique<TwoPlayerMode>();
    auto onePlayer = std::make_unique<OnePlayerMode>(); 
    auto playaswhite = std::make_unique<Playaswhite>(); 
    auto playasblack = std::make_unique<Playasblack>(); 

    twoPlayers->BoardSetUp(modify->getPiecePath(), modify->getBoard());

    while (window.isOpen()) {

        if (GameState == -1) {
            GameState = menu->handleEvents(window);
        }

        if (GameState == GamePlay_main) {
            GameState = mode->handleEvents(window);
        }

        if (GameState == Settings_main) {
            GameState = modify->handleEvent(window);
            if (GameState == Initial_main) {
                twoPlayers->BoardSetUp(modify->getPiecePath(), modify->getBoard());
            }
        }

        if (GameState == OnePlayer_main) {
            GameState = onePlayer->handleEvent(window);

            if (GameState == Playaswhite_main) {
                playaswhite->BoardSetUp(modify->getPiecePath(), modify->getBoard()); 
                playaswhite->GetDiffculty(onePlayer->EASY(), onePlayer->MEDIUM(), onePlayer->HARD());
            }

            if (GameState == Playasblack_main) {
                playasblack->BoardSetUp(modify->getPiecePath(), modify->getBoard());
                playasblack->GetDiffculty(onePlayer->EASY(), onePlayer->MEDIUM(), onePlayer->HARD());
            }
        }

        if (GameState == TwoPlayers_main) {
            GameState = twoPlayers->handleEvents(window);
        }

        if (GameState == Playasblack_main) {
            GameState = playasblack->handleEvents(window);
        }

        if (GameState == Playaswhite_main) {
            GameState = playaswhite->handleEvents(window); 
        }

        window.clear(Color(122, 125, 57));

        if (GameState == Initial_main) menu->draw(window);
        if (GameState == GamePlay_main) mode->draw(window);
        if (GameState == Settings_main) modify->draw(window);
        if (GameState == OnePlayer_main) onePlayer->draw(window); 
        if (GameState == TwoPlayers_main) twoPlayers->draw(window);
        if (GameState == Playaswhite_main)  playaswhite->draw(window); 
        if (GameState == Playasblack_main)  playasblack->draw(window); 

        window.display();
    }

    saveGameHistory();

    return 0;
}
