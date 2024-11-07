
#include "OnePlayerMode.h"
#include "Constant.h"

OnePlayerMode::OnePlayerMode() {
    // Load font
    if (!font.loadFromFile("Fonts/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
    }

    BoardTexture.loadFromFile("resize/Background.jpg");
    Board.setTexture(BoardTexture);

    easyButton.setSize(sf::Vector2f(200, 50));
    easyButton.setPosition(300, 100);
    easyButton.setFillColor(sf::Color::Blue);

    mediumButton.setSize(sf::Vector2f(200, 50));
    mediumButton.setPosition(300, 200);
    mediumButton.setFillColor(sf::Color::Blue);

    hardButton.setSize(sf::Vector2f(200, 50));
    hardButton.setPosition(300, 300);
    hardButton.setFillColor(sf::Color::Blue);

    backButton.setSize(sf::Vector2f(200, 50));
    backButton.setPosition(300, 400);
    backButton.setFillColor(sf::Color::Red);

    WhiteButton.setSize(sf::Vector2f(200, 50));
    BlackButton.setSize(sf::Vector2f(200, 50));
    BackTo.setSize(sf::Vector2f(200, 50));

    WhiteButton.setPosition(300, 150);
    BlackButton.setPosition(300, 250);
    BackTo.setPosition(300, 350);

    WhiteButton.setFillColor(sf::Color::Blue);
    BlackButton.setFillColor(sf::Color::Blue);
    BackTo.setFillColor(sf::Color::Red);

    selectMode = PlayasBlack = 0;
}

void OnePlayerMode::draw(RenderWindow& window) {
    Text easy, medium, hard, backText, White, Black, Backto; 

    easy.setFont(font);
    easy.setString("Easy");
    easy.setCharacterSize(24);

    medium.setFont(font);
    medium.setString("Medium");
    medium.setCharacterSize(24);


    hard.setFont(font);
    hard.setString("Hard");
    hard.setCharacterSize(24);


    backText.setFont(font);
    backText.setString("Back");
    backText.setCharacterSize(24);

    Backto.setFont(font);
    Backto.setString("Back");
    Backto.setCharacterSize(24);

    White.setFont(font);
    White.setString("White");
    White.setCharacterSize(24);

    Black.setFont(font);
    Black.setString("Black");
    Black.setCharacterSize(24);

    easy.setPosition(
        easyButton.getPosition().x + (easyButton.getSize().x - easy.getLocalBounds().width) / 2,
        easyButton.getPosition().y + (easyButton.getSize().y - easy.getLocalBounds().height) / 2 - 5
    );

    medium.setPosition(
        mediumButton.getPosition().x + (mediumButton.getSize().x - medium.getLocalBounds().width) / 2,
        mediumButton.getPosition().y + (mediumButton.getSize().y - medium.getLocalBounds().height) / 2 - 5
    );

    hard.setPosition(
        hardButton.getPosition().x + (hardButton.getSize().x - hard.getLocalBounds().width) / 2,
        hardButton.getPosition().y + (hardButton.getSize().y - hard.getLocalBounds().height) / 2 - 5
    );

    backText.setPosition(
        backButton.getPosition().x + (backButton.getSize().x - backText.getLocalBounds().width) / 2,
        backButton.getPosition().y + (backButton.getSize().y - backText.getLocalBounds().height) / 2 - 5
    );


    White.setPosition(
        WhiteButton.getPosition().x + (WhiteButton.getSize().x - White.getLocalBounds().width) / 2,
        WhiteButton.getPosition().y + (WhiteButton.getSize().y - White.getLocalBounds().height) / 2 - 5
    );

    
    Black.setPosition(
        BlackButton.getPosition().x + (BlackButton.getSize().x - Black.getLocalBounds().width) / 2,
        BlackButton.getPosition().y + (BlackButton.getSize().y - Black.getLocalBounds().height) / 2 - 5
    );


    Backto.setPosition(
        BackTo.getPosition().x + (BackTo.getSize().x - Backto.getLocalBounds().width) / 2,
        BackTo.getPosition().y + (BackTo.getSize().y - Backto.getLocalBounds().height) / 2 - 5
    );

    window.draw(Board);


    if (selectMode == 0) {
        window.draw(easyButton);
        window.draw(easy);

        window.draw(mediumButton);
        window.draw(medium);

        window.draw(hardButton);
        window.draw(hard);

        window.draw(backButton);
        window.draw(backText);
    }
    else {
        window.draw(WhiteButton); 
        window.draw(White);

        window.draw(BlackButton); 
        window.draw(Black); 

        window.draw(BackTo); 
        window.draw(Backto); 
    }


}

bool OnePlayerMode::EASY() {
    return Easy; 
}

bool OnePlayerMode::MEDIUM() {
    return Medium;
}

bool OnePlayerMode::HARD() {
    return Hard;
}

int OnePlayerMode::handleEvent(RenderWindow& window) {
    Event event;

    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) window.close();

        Vector2i pos = Mouse::getPosition(window);

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (selectMode) {
                if (WhiteButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    PlayasBlack = 0; selectMode = 0; 
                    return Playaswhite_main; 
                }

                if (BlackButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    PlayasBlack = 1; selectMode = 0;    
                    return Playasblack_main;
                }

                if (BackTo.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectMode = 0; 
                    return OnePlayer_main;
                }
            }
            else {
                if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return GamePlay_main;
                }

                if (easyButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectMode = 1; Easy = 1; Hard = Medium = 0; 
                    return OnePlayer_main;
                }

                if (mediumButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectMode = 1; Medium = 1; Hard = Easy = 0; 
                    return OnePlayer_main;
                }

                if (hardButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectMode = 1; Hard = 1, Medium = Easy = 0; 
                    return OnePlayer_main;
                }
            }
        }
    }

    return OnePlayer_main;
}
