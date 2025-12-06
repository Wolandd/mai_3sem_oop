#include "NPC.h"
#include <cassert>
#include <iostream>

void testCharacterFactory() {
    std::cout << "Testing NPCFactory..." << std::endl;
    auto knight = NPCFactory::createCharacter(NPCType::KNIGHT, "TestKnight", 100, 100);
    assert(knight->getCharacterName() == "TestKnight");
    assert(knight->getCharacterType() == NPCType::KNIGHT);
    assert(knight->getPositionX() == 100);
    assert(knight->getPositionY() == 100);
    assert(knight->checkIsActive() == true);
    auto squirrel = NPCFactory::createCharacter(NPCType::SQUIRREL, "TestSquirrel", 200, 200);
    assert(squirrel->getCharacterName() == "TestSquirrel");
    assert(squirrel->getCharacterType() == NPCType::SQUIRREL);
    auto pegasus = NPCFactory::createCharacter(NPCType::PEGASUS, "TestPegasus", 300, 300);
    assert(pegasus->getCharacterName() == "TestPegasus");
    assert(pegasus->getCharacterType() == NPCType::PEGASUS);
    std::cout << "NPCFactory tests passed!" << std::endl;
}

void testCharacterCanAttack() {
    std::cout << "\nTesting Character canAttack (Visitor pattern)..." << std::endl;
    auto knight = std::make_shared<Knight>("Knight1", 0, 0);
    auto squirrel = std::make_shared<Squirrel>("Squirrel1", 1, 1);
    auto pegasus = std::make_shared<Pegasus>("Pegasus1", 2, 2);
    assert(knight->canAttack(squirrel) == true);
    assert(squirrel->canAttack(pegasus) == true);
    assert(squirrel->canAttack(knight) == false);
    assert(pegasus->canAttack(knight) == false);
    assert(pegasus->canAttack(squirrel) == false);
    auto knight2 = std::make_shared<Knight>("Knight2", 3, 3);
    assert(knight->canAttack(knight2) == false);
    auto squirrel2 = std::make_shared<Squirrel>("Squirrel2", 4, 4);
    assert(squirrel->canAttack(squirrel2) == false);
    auto pegasus2 = std::make_shared<Pegasus>("Pegasus2", 5, 5);
    assert(pegasus->canAttack(pegasus2) == false);
    std::cout << "Character canAttack tests passed!" << std::endl;
}

void testDistance() {
    std::cout << "\nTesting distance calculation..." << std::endl;
    auto character1 = std::make_shared<Knight>("K1", 0, 0);
    auto character2 = std::make_shared<Knight>("K2", 3, 4);
    double dist = character1->calculateDistance(character2);
    assert(fabs(dist - 5.0) < 0.0001);
    std::cout << "Distance tests passed!" << std::endl;
}

void testSaveLoad() {
    std::cout << "\nTesting save/load functionality..." << std::endl;
    DungeonEditor editor;
    editor.addCharacter(std::make_shared<Knight>("SaveKnight", 10, 20));
    editor.addCharacter(std::make_shared<Squirrel>("SaveSquirrel", 30, 40));
    editor.addCharacter(std::make_shared<Pegasus>("SavePegasus", 50, 60));
    editor.serializeToFile("test_save.txt");
    DungeonEditor loadedEditor;
    loadedEditor.deserializeFromFile("test_save.txt");
    std::cout << "Save/load tests completed!" << std::endl;
    std::remove("test_save.txt");
}

void testBattle() {
    std::cout << "\nTesting battle system..." << std::endl;
    DungeonEditor editor;
    auto observer = std::make_shared<ConsoleObserver>();
    editor.addListener(observer);
    editor.addCharacter(std::make_shared<Knight>("BattleKnight", 0, 0));
    editor.addCharacter(std::make_shared<Squirrel>("BattleSquirrel", 10, 10));
    editor.addCharacter(std::make_shared<Pegasus>("BattlePegasus", 100, 100));
    std::cout << "Before battle:" << std::endl;
    editor.executeCombat(15);
    std::cout << "Battle tests completed!" << std::endl;
}

void testCoordinateValidation() {
    std::cout << "\nTesting coordinate validation..." << std::endl;
    bool exceptionThrown = false;
    try {
        auto invalid = std::make_shared<Knight>("Invalid", 600, 600);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown == true);
    std::cout << "Coordinate validation tests passed!" << std::endl;
}

int main() {
    std::cout << "=== Starting Unit Tests ===" << std::endl;
    testCharacterFactory();
    testCharacterCanAttack();
    testDistance();
    testSaveLoad();
    testBattle();
    testCoordinateValidation();
    std::cout << "\n=== All tests passed! ===" << std::endl;
    return 0;
}