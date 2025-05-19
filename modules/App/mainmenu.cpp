#include "app.h"

void MainMenu::handleEvents(App& app) {
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
                case sf::Keyboard::Key::Enter:
                    switch (currentSelection) {
                        case 0:
                            app.setState(States::Play);
                            break;
                        case 1:
                            app.setState(States::Settings);
                            break;
                        case 2:
                            window.close();
                            break;
                    }
                    break;
                case sf::Keyboard::Key::Tab:
                    currentSelection = (currentSelection + 1) % texts.size();
                    break;
                default:
                    break;
            }
        }
    }
}

void MainMenu::update(sf::RenderWindow& window, float deltaTime) {

}

void MainMenu::render(sf::RenderWindow& window) {
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
    window.display();

}
