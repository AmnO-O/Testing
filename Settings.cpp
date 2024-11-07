// Settings.cpp
#include "Settings.h"
#include "Constant.h"
#include <iostream>

vector<string>boardDesignPaths = {
    "images/BoardGame/background0.png","images/BoardGame/background5.png", "images/BoardGame/background6.png"
};

vector<string>pieceDesignPaths = { "images/FigureTheme/Cartoon/background.png",
"images/FigureTheme/Classic/background.png", "images/FigureTheme/Clear/background.png" };

Settings::Settings() {
    // Load font
    if (!font.loadFromFile("Fonts/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
    }

    soundEffectsOn = backgroundMusicOn = pieceTheme = musicAdjust = boardTheme = musicSoundThemes = 0;
    BoardTexture.loadFromFile("resize/Background.jpg");
    Board.setTexture(BoardTexture);

    selectedPieceThemeIndex = selectedBoardThemeIndex = 1; 

    for (int i = 0; i < 3; i++) {
        if (pieceDesignTextures[i].loadFromFile(pieceDesignPaths[i])) {
            pieceDesignOptions[i].setTexture(pieceDesignTextures[i]);
            pieceDesignOptions[i].setPosition(100 + i * 200, 200);
        }
        else {
            std::cout << "Failed to load " << pieceDesignPaths[i] << "\n";
        }
    }


    for (int i = 0; i < 3; i++) {
        if (boardDesignTextures[i].loadFromFile(boardDesignPaths[i])) {
            boardDesignOptions[i].setTexture(boardDesignTextures[i]);
            boardDesignOptions[i].setPosition(150 + i * 200, 200);
        }
        else {
            std::cout << "Failed to load " << boardDesignPaths[i] << "\n";
        }
    }

    boardColorText.setFont(font);
    boardColorText.setString("Board Theme");
    boardColorText.setCharacterSize(24);

    boardColorButton.setSize(sf::Vector2f(200, 50));
    boardColorButton.setPosition(300, 100);
    boardColorButton.setFillColor(sf::Color::Blue);

    pieceDesignText.setFont(font);
    pieceDesignText.setString("Piece Theme");
    pieceDesignText.setCharacterSize(24);

    pieceDesignButton.setSize(sf::Vector2f(200, 50));
    pieceDesignButton.setPosition(300, 200);
    pieceDesignButton.setFillColor(sf::Color::Blue);

    soundEffectsText.setFont(font);
    soundEffectsText.setString("Sound Effects");
    soundEffectsText.setCharacterSize(24);

    soundEffectsButton.setSize(sf::Vector2f(200, 50));
    soundEffectsButton.setPosition(300, 300);
    soundEffectsButton.setFillColor(sf::Color::Blue);

    backgroundMusicText.setFont(font);
    backgroundMusicText.setString("Background Music");
    backgroundMusicText.setCharacterSize(24);

    backgroundMusicButton.setSize(sf::Vector2f(200, 50));
    backgroundMusicButton.setPosition(300, 400);
    backgroundMusicButton.setFillColor(sf::Color::Blue);

    backButton.setSize(sf::Vector2f(100, 40));
    backButton.setPosition(20, 20);
    backButton.setFillColor(sf::Color::Red);

    boardColorText.setPosition(
        boardColorButton.getPosition().x + (boardColorButton.getSize().x - boardColorText.getLocalBounds().width) / 2,
        boardColorButton.getPosition().y + (boardColorButton.getSize().y - boardColorText.getLocalBounds().height) / 2 - 5
    );

    pieceDesignText.setPosition(
        pieceDesignButton.getPosition().x + (pieceDesignButton.getSize().x - pieceDesignText.getLocalBounds().width) / 2,
        pieceDesignButton.getPosition().y + (pieceDesignButton.getSize().y - pieceDesignText.getLocalBounds().height) / 2 - 5
    );

    soundEffectsText.setPosition(
        soundEffectsButton.getPosition().x + (soundEffectsButton.getSize().x - soundEffectsText.getLocalBounds().width) / 2,
        soundEffectsButton.getPosition().y + (soundEffectsButton.getSize().y - soundEffectsText.getLocalBounds().height) / 2 - 5
    );

    backgroundMusicText.setPosition(
        backgroundMusicButton.getPosition().x + (backgroundMusicButton.getSize().x - backgroundMusicText.getLocalBounds().width) / 2,
        backgroundMusicButton.getPosition().y + (backgroundMusicButton.getSize().y - backgroundMusicText.getLocalBounds().height) / 2 - 5
    );

    backButtonText.setFont(font);
    backButtonText.setString("Back");
    backButtonText.setCharacterSize(20);
    backButtonText.setFillColor(sf::Color::White);

    backButtonText.setPosition(
        backButton.getPosition().x + (backButton.getSize().x - backButtonText.getGlobalBounds().width) / 2,
        backButton.getPosition().y + (backButton.getSize().y - backButtonText.getGlobalBounds().height) / 4
    );
}

void Settings::draw(RenderWindow& window) {
    window.draw(Board);

    window.draw(backButton);
    window.draw(backButtonText);

    if (pieceTheme) {
        for (int i = 0; i < 3; i++) {
            if (i == selectedPieceThemeIndex) {
                sf::RectangleShape highlight(sf::Vector2f(pieceDesignOptions[i].getGlobalBounds().width, pieceDesignOptions[i].getGlobalBounds().height));
                highlight.setPosition(pieceDesignOptions[i].getPosition());
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Red);
                highlight.setOutlineThickness(3);
                window.draw(highlight);
            }

            window.draw(pieceDesignOptions[i]);
        }
        return;
    }

    if (boardTheme) {
        for (int i = 0; i < 3; i++) {
            if (i == selectedBoardThemeIndex) {
                sf::RectangleShape highlight(sf::Vector2f(boardDesignOptions[i].getGlobalBounds().width, boardDesignOptions[i].getGlobalBounds().height));
                highlight.setPosition(boardDesignOptions[i].getPosition());
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Red);
                highlight.setOutlineThickness(3);
                window.draw(highlight);
            }

            window.draw(boardDesignOptions[i]);
        }
        return;
    }

    window.draw(boardColorButton);
    window.draw(boardColorText);

    window.draw(pieceDesignButton);
    window.draw(pieceDesignText);

    window.draw(soundEffectsButton);
    window.draw(soundEffectsText);

    window.draw(backgroundMusicButton);
    window.draw(backgroundMusicText);
}

int Settings::handleEvent(RenderWindow& window) {
    Event event;

    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) window.close();

        Vector2i pos = Mouse::getPosition(window);

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                if (pieceTheme) {
                    pieceTheme = 0;
                    return Settings_main;
                }

                if (boardTheme) {
                    boardTheme = 0;
                    return Settings_main;
                }

                return Initial_main; 
            }

            bool comb = pieceTheme | boardTheme;

            if (soundEffectsButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {

            }

            if (backgroundMusicButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {

            }

            if (pieceTheme) {
                for (int i = 0; i < 3; i++) {
                    if (pieceDesignOptions[i].getGlobalBounds().contains(pos.x, pos.y)) {
                        selectedPieceThemeIndex = i; 
                    }
                }
            }

            if (boardTheme) {
                for (int i = 0; i < 3; i++) {
                    if (boardDesignOptions[i].getGlobalBounds().contains(pos.x, pos.y)) {
                        selectedBoardThemeIndex = i;
                    }
                }
            }

            if (comb == 0 && pieceDesignButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) pieceTheme = 1;

            if (comb == 0 && boardColorButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) boardTheme = 1;

        }
    }

    return Settings_main;
}
string Settings::getPiecePath() {
    string path = ""; 
    for (char c : pieceDesignPaths[selectedPieceThemeIndex]) {
        if (c == 'b') return path; 
        path += c; 
    }

    return path; 
}

char Settings::getBoard() {
    for (char c : boardDesignPaths[selectedBoardThemeIndex]) {
        if (isdigit(c)) return c;
    }

    return '0';
}