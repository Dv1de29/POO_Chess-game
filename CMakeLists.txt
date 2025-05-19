cmake_minimum_required(VERSION 3.5)
project(Clion_POO LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(Clion_POO
        main.cpp
        modules/App/app.cpp
        modules/App/mainmenu.cpp
        modules/App/settings.cpp
        modules/App/play.cpp
        modules/board/gameboard.cpp
        modules/Pieces/piece.cpp
        modules/History/history.cpp
        modules/ResourceManager/resource_manager.cpp
        )
target_compile_features(Clion_POO PRIVATE cxx_std_17)
target_link_libraries(Clion_POO PRIVATE SFML::Graphics SFML::Audio)