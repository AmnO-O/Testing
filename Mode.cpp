// Mode.cpp
#include "Mode.h"
#include "Constant.h"

Mode::Mode() {
    if (!font.loadFromFile("Fonts/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        std::exit(-1);
    }

    BoardTexture.loadFromFile("resize/Background.jpg");
    Board.setTexture(BoardTexture);

    One_Player.setSize(sf::Vector2f(200, 50));
    Two_Players.setSize(sf::Vector2f(200, 50));
    Back.setSize(sf::Vector2f(200, 50));

    One_Player.setPosition(300, 150);
    Two_Players.setPosition(300, 250);
    Back.setPosition(300, 350);

    One_Player.setFillColor(sf::Color::Blue);
    Two_Players.setFillColor(sf::Color::Blue);
    Back.setFillColor(sf::Color::Red);

    playText.setString("Human vs BOT");
    settingsText.setString("Human vs Human");
    exitText.setString("Back to menu");

    playText.setFont(font);
    settingsText.setFont(font);
    exitText.setFont(font);

    playText.setCharacterSize(24);
    settingsText.setCharacterSize(24);
    exitText.setCharacterSize(24);

    playText.setPosition(
        One_Player.getPosition().x + (One_Player.getSize().x - playText.getLocalBounds().width) / 2,
        One_Player.getPosition().y + (One_Player.getSize().y - playText.getLocalBounds().height) / 2 - 5
    );

    settingsText.setPosition(
        Two_Players.getPosition().x + (Two_Players.getSize().x - settingsText.getLocalBounds().width) / 2,
        Two_Players.getPosition().y + (Two_Players.getSize().y - settingsText.getLocalBounds().height) / 2 - 5
    );

    exitText.setPosition(
        Back.getPosition().x + (Back.getSize().x - exitText.getLocalBounds().width) / 2,
        Back.getPosition().y + (Back.getSize().y - exitText.getLocalBounds().height) / 2 - 5
    );
}

bool Mode::isMouseOver(const sf::RectangleShape& button, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    return buttonBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

int Mode::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (isMouseOver(One_Player, window)) {
                std::cout << "Play button clicked!\n";
                return OnePlayer_main;
            }
            else if (isMouseOver(Two_Players, window)) {
                std::cout << "Settings button clicked!\n";
                return TwoPlayers_main;
            }
            else if (isMouseOver(Back, window)) {
                std::cout << "Exit button clicked!\n";
                return -1; 
            }
        }
    }

    return GamePlay_main;
}

void Mode::draw(sf::RenderWindow& window) {
    window.draw(Board);
    window.draw(One_Player);
    window.draw(Two_Players);
    window.draw(Back);
    window.draw(playText);
    window.draw(settingsText);
    window.draw(exitText);
}
