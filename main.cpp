#include "main.h"
#include <iostream>
#include "engine/engine.h"
int main() {
    try {
        Engine engine;
        engine.MainLoop();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}