#include "NPC.h"
#include <memory>

int main() {
    try {
        DungeonEditor editor;
        auto consoleObserver = std::make_shared<ConsoleObserver>();
        auto fileObserver = std::make_shared<FileObserver>();
        editor.addListener(consoleObserver);
        editor.addListener(fileObserver);
        std::cout << "=== Creating Characters using Factory ===" << std::endl;
        auto knight1 = NPCFactory::createCharacter(NPCType::KNIGHT, "Arthur", 100, 100);
        auto knight2 = NPCFactory::createCharacter(NPCType::KNIGHT, "Lancelot", 150, 150);
        auto squirrel1 = NPCFactory::createCharacter(NPCType::SQUIRREL, "Nutty", 50, 50);
        auto squirrel2 = NPCFactory::createCharacter(NPCType::SQUIRREL, "Fluffy", 120, 120);
        auto pegasus1 = NPCFactory::createCharacter(NPCType::PEGASUS, "Storm", 200, 200);
        auto pegasus2 = NPCFactory::createCharacter(NPCType::PEGASUS, "Sky", 250, 250);
        editor.addCharacter(knight1);
        editor.addCharacter(knight2);
        editor.addCharacter(squirrel1);
        editor.addCharacter(squirrel2);
        editor.addCharacter(pegasus1);
        editor.addCharacter(pegasus2);
        editor.displayAll();
        std::cout << "\n=== Saving to file ===" << std::endl;
        editor.serializeToFile("dungeon.txt");
        std::cout << "Saved to dungeon.txt" << std::endl;
        std::cout << "\n=== Loading from file ===" << std::endl;
        DungeonEditor loadedEditor;
        loadedEditor.addListener(consoleObserver);
        loadedEditor.addListener(fileObserver);
        loadedEditor.deserializeFromFile("dungeon.txt");
        loadedEditor.displayAll();
        std::cout << "\n=== First Battle ===" << std::endl;
        loadedEditor.executeCombat(100);
        loadedEditor.displayAll();
        std::cout << "\n=== Second Battle ===" << std::endl;
        auto squirrel3 = NPCFactory::createCharacter(NPCType::SQUIRREL, "Chipper", 300, 300);
        auto pegasus3 = NPCFactory::createCharacter(NPCType::PEGASUS, "Wind", 310, 310);
        loadedEditor.addCharacter(squirrel3);
        loadedEditor.addCharacter(pegasus3);
        loadedEditor.executeCombat(200);
        loadedEditor.displayAll(); 
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}