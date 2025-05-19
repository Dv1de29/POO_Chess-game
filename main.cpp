#include <iostream>
#include "./modules/App/app.h"

int main() {
    std::cout << "Hello Squirell!" << std::endl;

    App::getInstance().run();
}