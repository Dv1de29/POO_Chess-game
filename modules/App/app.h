#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include "SFML/Audio/Sound.hpp"

#include "../board/gameboard.h"
#include "../ResourceManager/resource_manager.h"

#include <optional>


class App;

enum class States {
    MainMenu,
    Settings,
    Play,
};

// GameState class that is the base for all the game states

class GameState{
protected:
    ResourceManager<sf::Font> fonts_resource;
    std::shared_ptr<sf::Font> romanFont;
    std::shared_ptr<sf::Font> normalFont;
    bool audioMode = true;
public:
    GameState() /* : romanFont("./assets/LuxuriousRoman-Regular.ttf") */ {
        romanFont = fonts_resource.load("Roman font","../assets/LuxuriousRoman-Regular.ttf");
        normalFont = fonts_resource.load("Normal Font", "../assets/arial.ttf");
    }
    virtual ~GameState() = default;
    virtual void handleEvents(App& app) = 0;
    virtual void update(sf::RenderWindow& window, float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    bool isAudio() const;
    void setAudio(bool audio);
};

class MainMenu : public GameState {
    std::vector<sf::Text> texts;
    int currentSelection = 0;
public:
    MainMenu() {
        for ( int i = 0; i < 3; i++) {
            sf::Text text(*romanFont);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::White);
            text.setStyle(sf::Text::Bold);
            switch (i) {
                case 0:
                    text.setString("Start Game");
                    break;
                case 1:
                    text.setString("Settings");
                    break;
                case 2:
                    text.setString("Quit");
                    break;
                default:
                    break;
            }
            texts.push_back(text);
        }
    }
    ~MainMenu() override = default;

    void handleEvents(App& app) override;
    void update(sf::RenderWindow& window, float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};

class Settings : public GameState {
    std::vector<sf::Text> texts;
    std::vector<sf::Text> options;
    int currentSelection = 0;
    int currentOption = 0;
    bool inModesSelection = false;
    int currentMode = 0;
public:
    Settings() {
        for ( int i = 0; i < 3; i++) {
            sf::Text text(*romanFont);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::White);
            text.setStyle(sf::Text::Bold);

            sf::Text option(*romanFont);
            option.setCharacterSize(20);
            option.setFillColor(sf::Color::Black);
            option.setStyle(sf::Text::Italic);
            switch (i) {
                case 0:
                    text.setString("Game Modes");
                    option.setString("Normal Mode");
                break;
                case 1:
                    text.setString("Audio: ON");
                    option.setString("Duck Mode");
                break;
                case 2:
                    text.setString("Return to Main Menu");
                break;
                default:
                    break;
            }
            texts.push_back(text);
            if ( i != 2) {
                options.push_back(option);
            }
        }
    }
    ~Settings() override = default;

    void handleEvents(App& app) override;
    void update(sf::RenderWindow& window, float deltaTime) override;
    void render(sf::RenderWindow& window) override;

};

class Play : public GameState {
    GameBoard gameBoard;

    bool gameOver = false;
    float whiteTime = 600;
    float blackTime = 600;

    std::shared_ptr<sf::Font> timefont;


public:
    Play() {
        timefont = fonts_resource.load("Time Font", "../assets/Pro Display tfb.ttf");
    }
    ~Play() override = default;

    Play(const Play&) = delete;
    Play& operator=(const Play&) = delete;
    Play(Play&&) noexcept = default;
    Play& operator=(Play&&) noexcept = default;


    void handleEvents(App& app) override;
    void update(sf::RenderWindow& window, float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void setGameOver(bool gameOver);
    bool isGameOver() const;
};


class SoundManager {
    std::vector<sf::Sound> activeSounds;
    bool audioEnabled = true;

    public:
    SoundManager() = default;

    void setAudio(bool audio);
    bool isAudio() const;
    void playSound(const sf::SoundBuffer& buffer);
    void cleanSounds();
};


// App class that is the engine
class App {
    sf::VideoMode videoMode;
    sf::RenderWindow window;
    std::optional<sf::Event> ev;
    // States currentState;
    std::unique_ptr<GameState> currentState;

    // bool audio = true;
    // std::vector<sf::Sound> activeSounds;
    SoundManager soundManager;

    int gameModeSelection = 0;

    App();
    void handleEvents();
    void update(float deltaTime);
    void render();

protected:
    ~App() = default;

public:
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    static App& getInstance();
    void run();
    sf::RenderWindow& getWindow();
    void setState(States state);
    void setAudio(bool audio);
    bool isAudio() const;
    int getMode() const;
    void setMode(int mode);

    void playSound(const sf::SoundBuffer& buffer);
};

#endif // APP_HPP
