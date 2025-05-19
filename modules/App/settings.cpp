#include "app.h"

void Settings::handleEvents(App& app) {
    sf::RenderWindow& window = app.getWindow();
    while (const std::optional ev = window.pollEvent()) {
        if ( ev->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (auto keyPressed = ev->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
                case sf::Keyboard::Key::Q:
                    window.close();
                break;
                case sf::Keyboard::Key::Escape:
                    app.setState(States::MainMenu);
                break;
                case sf::Keyboard::Key::Enter:
                    switch (currentSelection) {
                        case 0:
                            if ( inModesSelection == true ) {
                                app.setMode(this->currentMode);
                            }
                            else {
                                this->currentMode = app.getMode();
                            }
                            this->inModesSelection = !inModesSelection;
                            break;
                        case 1:
                            app.setAudio( !app.isAudio() );
                            texts[1].setString( app.isAudio() ? "Audio: ON" : "Audio: OFF" );
                            break;
                        case 2:
                            app.setMode(currentMode);
                            app.setState(States::MainMenu);
                            break;
                        default:
                            break;
                    }
                break;
                case sf::Keyboard::Key::Tab:
                    if ( inModesSelection == true ) {
                        this->currentMode = (this->currentMode + 1) % this->options.size();
                    }
                    else {
                        currentSelection = (currentSelection + 1) % texts.size();
                    }
                break;
                default:
                    break;
            }
        }
    }
}

void Settings::update(sf::RenderWindow& window, float deltaTime) {

}

void Settings::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    for ( int i = 0; i < texts.size(); i++) {
        if ( i == currentSelection ) {
            sf::FloatRect textRect = texts[i].getGlobalBounds();
            sf::RectangleShape background;
            background.setFillColor(sf::Color(50,50,50));
            background.setSize(sf::Vector2f(textRect.size.x + 20, textRect.size.y + 20));
            background.setPosition(sf::Vector2f(textRect.position.x - 10,textRect.position.y - 10));
            window.draw(background);
        }
        texts[i].setPosition(sf::Vector2f((window.getSize().x - (texts[i].getCharacterSize() * texts[i].getString().getSize() * 0.5)) * 0.5, window.getSize().y * 0.3 + i * window.getSize().y * 0.1));
        window.draw(texts[i]);
    }

    if ( inModesSelection ) {
        sf::Vector2f textRect(
            texts[0].getPosition().x + texts[0].getCharacterSize() * (texts[0].getString().getSize() - 2),
            texts[0].getPosition().y - texts[0].getCharacterSize() * 0.5);
        sf::RectangleShape background;
        background.setFillColor(sf::Color(176, 169, 169));
        background.setSize(sf::Vector2f(options[0].getCharacterSize() * options[0].getString().getSize() + 10,options[0].getCharacterSize() * (options.size() + 2)));
        background.setPosition(sf::Vector2f(textRect.x, textRect.y));
        window.draw(background);

        for ( int i = 0; i < options.size(); i++) {
            options[i].setPosition(sf::Vector2f(textRect.x + 5, textRect.y + i * (options[i].getCharacterSize() * 1.7) + 15));

            if ( i == currentMode ) {
                sf::FloatRect textRect = options[i].getGlobalBounds();
                sf::RectangleShape background;
                background.setFillColor(sf::Color(84, 81, 81));
                background.setSize(sf::Vector2f(textRect.size.x + 10, textRect.size.y + 10));
                background.setPosition(sf::Vector2f(textRect.position.x - 5,textRect.position.y - 5));
                window.draw(background);
            }
            window.draw(options[i]);
        }


    }
    window.display();
}