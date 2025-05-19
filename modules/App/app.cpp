#include "app.h"

App::App()
    : videoMode(sf::Vector2u(1280u,720u), 32),
      window(this->videoMode, "SFML game", sf::Style::Titlebar | sf::Style::Close),
      currentState(nullptr) {
    setState(States::MainMenu);
    window.setFramerateLimit(120);



    auto piece = std::make_unique<Pawn>("white");
}

App& App::getInstance() {
    static App instance;
    return instance;
}

void App::handleEvents() {
    if ( this->currentState ) {
        currentState->handleEvents(*this);
    }
}

void App::update(float deltaTime) {
    if ( this->currentState ) {
        this->currentState->update(this->getWindow(), deltaTime);
    }
}

void App::render() {
    // clear of the window
    if ( this->currentState ) {
        currentState->render(this->getWindow());
    }
}

void App::run() {
    sf::Clock clock;

    // sf::SoundBuffer* selectedbuffer = nullptr;
    while (this->window.isOpen()) {
        /////// Managing settings
        this->currentState->setAudio(this->isAudio());

        /////Handle events for respective GameState
        handleEvents();

        ///// Updating based on framerate
        float deltaTime = clock.restart().asSeconds();
        update(deltaTime);


        ///// Cleaning the stopped sounds
        this->soundManager.cleanSounds();

        ///// Rendering the window
        render();
    }
}

sf::RenderWindow& App::getWindow() {
    return window;
}

void App::setState(States state) {
    switch (state) {
        case States::MainMenu:
            currentState = std::make_unique<MainMenu>();
            break;
        case States::Play:
            currentState = std::make_unique<Play>();
            break;
        case States::Settings:
            currentState = std::make_unique<Settings>();
            break;
    }
}

void App::setAudio(bool audio) {
    this->soundManager.setAudio(audio);
}

bool App::isAudio() const{
    return this->soundManager.isAudio();
}

int App::getMode() const{
    return gameModeSelection;
}


void App::setMode(int mode) {
    this->gameModeSelection = mode;
}

void App::playSound(const sf::SoundBuffer &buffer) {
    soundManager.playSound(buffer);
}




void SoundManager::setAudio(bool audio) { this->audioEnabled = audio; }
bool SoundManager::isAudio() const { return this->audioEnabled; }

void SoundManager::playSound(const sf::SoundBuffer &buffer) {
    if ( !this->audioEnabled) return;
    this->activeSounds.emplace_back(buffer);
    this->activeSounds.back().play();
}

void SoundManager::cleanSounds() {
    this->activeSounds.erase( std::remove_if(this->activeSounds.begin(), this->activeSounds.end(),
        [](const sf::Sound& sound) {
            return sound.getStatus() == sf::Sound::Status::Stopped;
        }),
        this->activeSounds.end() );
}





bool GameState::isAudio() const { return this->audioMode; }

void GameState::setAudio(bool audio) {
    this->audioMode = audio;
}



