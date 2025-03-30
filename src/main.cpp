#include <iostream>
#include <exception>
#include "Game.h"

int main() {
    try {
        // Create and run the game
        Game game;
        game.run();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred!" << std::endl;
        return 1;
    }
} 