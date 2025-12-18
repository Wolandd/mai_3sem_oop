#include "NPC.h"

int main() {
    try {
        GameEngine game;
        
        auto consoleObserver = std::make_shared<ConsoleObserver>();
        auto fileObserver = std::make_shared<FileObserver>();
        
        game.addObserver(consoleObserver);
        game.addObserver(fileObserver);
        
        game.initializeGame(50);
        
        game.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}