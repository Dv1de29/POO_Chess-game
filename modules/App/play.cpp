#include "app.h"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

void Play::handleEvents(App& app) {
    sf::RenderWindow& window = app.getWindow();
    while ( const std::optional ev = window.pollEvent()) {
        if ( ev->is<sf::Event::Closed>()) {
            window.close();
        }
        else if ( auto keyPressed = ev->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
                case sf::Keyboard::Key::Q:
                    window.close();
                break;
                case sf::Keyboard::Key::Escape:
                    app.setState(States::MainMenu);
                break;
                case sf::Keyboard::Key::Enter:
                    ////// Check for posibilities of moves
                    if ( gameBoard.getSelectedBox() == true ) {
                        gameBoard.setSelectedBox(false);
                        std::vector<Coord> moves = gameBoard.legalMoves(gameBoard.getSelectedCoord());
                        for ( auto &move : moves ) {
                            if ( gameBoard.getCoord() == move ) {
                                ///// move the piece
                                gameBoard.movePiece(gameBoard.getSelectedCoord(), move);
                                gameBoard.setSelectedBox(false);
                                if ( gameBoard.getTurn() == "white") {
                                    gameBoard.setTurn("black");
                                }
                                else {
                                    gameBoard.setTurn("white");
                                }

                                sf::SoundBuffer checkbuffer;
                                sf::SoundBuffer movebuffer;
                                if ( !checkbuffer.loadFromFile("../assets/move-check.wav") ) {
                                    throw std::runtime_error("Failed to load movesound buffer check");
                                }
                                if ( !movebuffer.loadFromFile("../assets/move-self.wav") ) {
                                    throw std::runtime_error("Failed to load movesound buffer move");
                                }

                                sf::SoundBuffer* selectedBuffer = nullptr;
                                if ( gameBoard.inCheck() == true ) {
                                    selectedBuffer = &checkbuffer;
                                } else {
                                    selectedBuffer = &movebuffer;
                                }
                                app.playSound(*selectedBuffer);


                                break;
                            }
                        }
                    }
                    else {
                        if ( auto* piece = gameBoard.getPiece(gameBoard.getCoord().row, static_cast<int>( gameBoard.getCoord().col - 'a' )).get();
                            piece && piece->getColor() == gameBoard.getTurn() ) {
                            gameBoard.setSelectedBox(true);
                            gameBoard.setSelectedCoord(gameBoard.getCoord().col, gameBoard.getCoord().row);
                        }
                        else {
                            break;
                        }
                    }
                    break;
                case sf::Keyboard::Key::Up:
                    gameBoard.setCoordRow((gameBoard.getCoord().row + 1) % 8);
                break;
                case sf::Keyboard::Key::Down:
                    gameBoard.setCoordRow((gameBoard.getCoord().row + 7) % 8);
                break;
                case sf::Keyboard::Key::Right: {
                    char col = gameBoard.getCoord().col;
                    col++;
                    if ( col == 'i' ) {
                        col = 'a';
                    }
                    gameBoard.setCoordCol(col);
                    break;
                }
                case sf::Keyboard::Key::Left: {
                    char col = gameBoard.getCoord().col;
                    if ( col == 'a' ) {
                        col = 'i';
                    }
                    col--;
                    gameBoard.setCoordCol(col);
                    break;
                }
                default:
                    break;
            }
        }
        else if ( auto mousePressed = ev->getIf<sf::Event::MouseButtonPressed>()) {
            if ( gameBoard.inBoard(mousePressed->position.x, mousePressed->position.y) ) {
                std::cout << "VEVERITA A NIMERIT TABLA\n";
                Coord boxPressed = gameBoard.getCoordForMouse(mousePressed->position.x, mousePressed->position.y);
                gameBoard.setCoord(boxPressed.col, boxPressed.row);
                ////// Check for posibilities of moves
                if ( gameBoard.getSelectedBox() == true ) {
                    gameBoard.setSelectedBox(false);
                    std::vector<Coord> moves = gameBoard.legalMoves(gameBoard.getSelectedCoord());
                    for ( auto &move : moves ) {
                        if ( gameBoard.getCoord() == move ) {
                            ///// move the piece
                            gameBoard.movePiece(gameBoard.getSelectedCoord(), move);
                            gameBoard.setSelectedBox(false);
                            if ( gameBoard.getTurn() == "white") {
                                gameBoard.setTurn("black");
                            }
                            else {
                                gameBoard.setTurn("white");
                            }

                            sf::SoundBuffer checkbuffer;
                            sf::SoundBuffer movebuffer;
                            if ( !checkbuffer.loadFromFile("../assets/move-check.wav") ) {
                                std::cerr << "Failed to load check buffer sound";
                            }
                            if ( !movebuffer.loadFromFile("../assets/move-self.wav") ) {
                                std::cerr << "Failed to load move buffer sound";
                            }

                            sf::SoundBuffer* selectedBuffer = nullptr;
                            if ( gameBoard.inCheck() == true ) {
                                selectedBuffer = &checkbuffer;
                            } else {
                                selectedBuffer = &movebuffer;
                            }
                            app.playSound(*selectedBuffer);

                            break;
                        }
                    }
                }
                else {
                    if ( auto* piece = gameBoard.getPiece(gameBoard.getCoord().row, static_cast<int>( gameBoard.getCoord().col - 'a' )).get();
                        piece && piece->getColor() == gameBoard.getTurn() ) {
                        gameBoard.setSelectedBox(true);
                        gameBoard.setSelectedCoord(gameBoard.getCoord().col, gameBoard.getCoord().row);
                        }
                    else {
                        break;
                    }
                }
            } else {
                std::cout << "VEVERITA NU A NIMERIT TABLA :((((\n";
            }

        }
    }
}

void Play::update(sf::RenderWindow& window, float deltaTime) {
    if ( gameBoard.isCheckMate() == true ) {
        this->gameOver = true;
    }
    else {
        if ( gameBoard.getTurn() == "white" ) {
            whiteTime -= deltaTime;
        }
        else {
            blackTime -= deltaTime;
        }
    }
}

void Play::render(sf::RenderWindow& window) {
    window.clear();

    ///// Drawing the board

    float boardFromWindowY = window.getSize().y - 40;
    sf::RectangleShape backBoard(sf::Vector2f(boardFromWindowY, boardFromWindowY));
    backBoard.setFillColor(sf::Color::Black);
    backBoard.setOutlineColor(sf::Color::White);
    backBoard.setOutlineThickness(3);
    backBoard.setPosition(sf::Vector2f((window.getSize().x - boardFromWindowY) * 0.5, 20));
    window.draw(backBoard);



    float chessBoxSize = (boardFromWindowY - 20) / 8;

    gameBoard.setPos(backBoard.getPosition().x + 10, backBoard.getPosition().y + 10);
    gameBoard.setSize(boardFromWindowY - 20, boardFromWindowY - 20);

    std::vector<Coord> moves;
    if ( gameBoard.getSelectedBox() ) {
        std::cout << "Veverita a selectat un patrat\n";
        moves = gameBoard.legalMoves(gameBoard.getSelectedCoord());
    }

    // for ( Coord move : moves ) {
    //     std::cout << move.col << move.row << std::endl;
    // }

    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {
            sf::RectangleShape chessBox;
            chessBox.setSize(sf::Vector2f(chessBoxSize, chessBoxSize));
            chessBox.setPosition(sf::Vector2f(backBoard.getPosition().x + 10 + j * chessBoxSize, backBoard.getPosition().y + 10 + i * chessBoxSize));
            if ( (8 * i + j) % 2 == 0 ) {
                if ( i % 2 == 0 ) {
                    chessBox.setFillColor(sf::Color(150, 75, 0));
                }
                else {
                    chessBox.setFillColor(sf::Color::White);
                }
            }
            else {
                if ( i % 2 == 1 ) {
                    chessBox.setFillColor(sf::Color(150, 75, 0));
                }
                else {
                    chessBox.setFillColor(sf::Color::White);
                }
            }

            if ( gameBoard.getSelectedCoord() == Coord{static_cast<char>('a' + j), 7-i}) {
                if ( gameBoard.getSelectedBox() ) {
                    chessBox.setFillColor(sf::Color(60,60,60
                        ));
                }
            }

            if ( gameBoard.getSelectedBox() == true && moves.size() > 0 ) {
                for ( Coord &move : moves ) {
                    if ( Coord{static_cast<char>('a' + j), 7-i} == move ) {
                        chessBox.setFillColor(sf::Color::Red);
                    }
                }
            }

            if ( gameBoard.getCoord() == Coord{static_cast<char>('a' + j), 7-i}) {
                chessBox.setFillColor(sf::Color::Blue);
            }

            window.draw(chessBox);

            if ( auto& piece = gameBoard.getPiece(7-i, j) ) {
                sf::Sprite pieceSprite(piece->getTexture());
                pieceSprite.setPosition(chessBox.getPosition());
                float spriteSize = chessBox.getSize().y / piece->getTexture().getSize().y;
                pieceSprite.setScale(sf::Vector2f(spriteSize, spriteSize));
                if ( gameBoard.isCheckMate() == true && piece->getColor() == gameBoard.getTurn() ) {
                    if ( auto* king = dynamic_cast<King*>(piece.get())) {
                        pieceSprite.setColor(sf::Color(70,70,70));
                        sf::FloatRect bound = pieceSprite.getGlobalBounds();
                        pieceSprite.setOrigin(sf::Vector2f(bound.size.x, bound.size.y));
                        pieceSprite.setPosition(sf::Vector2f(chessBox.getPosition().x + bound.size.x * 0.5, chessBox.getPosition().y + bound.size.y * 0.5));
                        pieceSprite.setRotation(sf::degrees(90));
                    }
                }
                if ( gameBoard.isCheckMate() == true && piece->getColor() != gameBoard.getTurn() ) {
                    if ( auto* king = dynamic_cast<King*>(piece.get())) {
                        pieceSprite.setColor(sf::Color(255, 215, 0));
                    }
                }
                window.draw(pieceSprite);
            }

        }
    }
    moves.clear();


    //// Drawing the time boxes

    sf::Text textTime(*(this->timefont));
    textTime.setCharacterSize(30);
    textTime.setStyle(sf::Text::Italic);


    sf::RectangleShape blackTimeBox;
    blackTimeBox.setSize(sf::Vector2f(200, textTime.getCharacterSize() + 20));
    blackTimeBox.setPosition(sf::Vector2f{backBoard.getPosition().x + backBoard.getSize().x + 10, backBoard.getPosition().y});
    blackTimeBox.setOutlineColor(sf::Color::White);
    blackTimeBox.setOutlineThickness(3);
    blackTimeBox.setFillColor(sf::Color::Black);
    window.draw(blackTimeBox);

    sf::RectangleShape whiteTimeBox;
    whiteTimeBox.setSize(sf::Vector2f(200, textTime.getCharacterSize() + 20));
    whiteTimeBox.setPosition(sf::Vector2f{backBoard.getPosition().x + backBoard.getSize().x + 10, backBoard.getPosition().y + backBoard.getSize().y - whiteTimeBox.getSize().y});
    whiteTimeBox.setOutlineColor(sf::Color::White);
    whiteTimeBox.setOutlineThickness(3);
    whiteTimeBox.setFillColor(sf::Color::Black);
    window.draw(whiteTimeBox);

    textTime.setPosition(sf::Vector2f{whiteTimeBox.getPosition().x + 10, whiteTimeBox.getPosition().y + 10});
    int hours = static_cast<int>(this->whiteTime) / 3600;
    int minutes = static_cast<int>(this->whiteTime) % 3600 / 60;
    int seconds = static_cast<int>(this->whiteTime) % 60;
    if ( hours > 0) {
        textTime.setString(std::to_string(hours) + ":" + (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
    }
    else {
        textTime.setString((minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
    }
    window.draw(textTime);

    textTime.setPosition(sf::Vector2f{blackTimeBox.getPosition().x + 10, blackTimeBox.getPosition().y + 10});
    hours = static_cast<int>(this->blackTime) / 3600;
    minutes = static_cast<int>(this->blackTime) % 3600 / 60;
    seconds = static_cast<int>(this->blackTime) % 60;
    if ( hours > 0) {
        textTime.setString(std::to_string(hours) + ":" + (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
    }
    else {
        // textTime.setString(minutes + ":" + seconds);
        textTime.setString((minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
    }
    window.draw(textTime);


    //// Drawing movesText
    sf::RectangleShape movestextBox;
    movestextBox.setPosition(sf::Vector2f(blackTimeBox.getPosition().x, blackTimeBox.getPosition().y + blackTimeBox.getSize().y + 10));
    movestextBox.setSize(sf::Vector2f(whiteTimeBox.getSize().x,whiteTimeBox.getPosition().y - blackTimeBox.getPosition().y - blackTimeBox.getSize().y - 20));
    // movestextBox.setSize(sf::Vector2f(200,200));
    movestextBox.setOutlineColor(sf::Color::White);
    movestextBox.setOutlineThickness(3);
    movestextBox.setFillColor(sf::Color::Black);
    window.draw(movestextBox);

    sf::Text movesText(*normalFont);
    movesText.setCharacterSize(20);
    movesText.setStyle(sf::Text::Italic);
    movesText.setPosition(sf::Vector2f(movestextBox.getPosition().x + 10, movestextBox.getPosition().y + 10));
    movesText.setString(gameBoard.getMovesString());
    window.draw(movesText);

    window.display();
}

void Play::setGameOver(const bool gameOver) {
    this->gameOver = gameOver;
}

bool Play::isGameOver() const {
    return this->gameOver;
}
