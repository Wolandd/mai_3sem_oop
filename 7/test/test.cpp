#include "NPC.h"
#include <cassert>
#include <iostream>

void testNPCProperties() {
    std::cout << "Testing NPC properties..." << std::endl;
    
    Knight knight("TestKnight", 50, 50);
    assert(knight.getMoveRange() == 30);
    assert(knight.getAttackRange() == 10);
    assert(knight.canAttack(NPCType::SQUIRREL) == true);
    assert(knight.canAttack(NPCType::KNIGHT) == false);
    assert(knight.canAttack(NPCType::PEGASUS) == false);
    assert(knight.getSymbol() == 'K');
    
    Squirrel squirrel("TestSquirrel", 60, 60);
    assert(squirrel.getMoveRange() == 5);
    assert(squirrel.getAttackRange() == 10);
    assert(squirrel.canAttack(NPCType::PEGASUS) == true);
    assert(squirrel.canAttack(NPCType::SQUIRREL) == false);
    assert(squirrel.canAttack(NPCType::KNIGHT) == false);
    assert(squirrel.getSymbol() == 'S');
    
    Pegasus pegasus("TestPegasus", 70, 70);
    assert(pegasus.getMoveRange() == 30);
    assert(pegasus.getAttackRange() == 10);
    assert(pegasus.canAttack(NPCType::KNIGHT) == false);
    assert(pegasus.canAttack(NPCType::SQUIRREL) == false);
    assert(pegasus.canAttack(NPCType::PEGASUS) == false);
    assert(pegasus.getSymbol() == 'P');
    
    std::cout << "NPC properties tests passed!" << std::endl;
}

void testMovement() {
    std::cout << "\nTesting NPC movement..." << std::endl;
    
    Knight knight("MoveKnight", 10, 10);
    knight.performMove();
    
    int x = knight.getPositionX();
    int y = knight.getPositionY();
    assert(x >= 0 && x < FIELD_WIDTH);
    assert(y >= 0 && y < FIELD_HEIGHT);
    
    std::cout << "Movement test passed!" << std::endl;
}

void testDistance() {
    std::cout << "\nTesting distance calculations..." << std::endl;
    
    auto npc1 = std::make_shared<Knight>("K1", 0, 0);
    auto npc2 = std::make_shared<Knight>("K2", 3, 4);
    
    double dist = npc1->calculateDistance(npc2);
    assert(fabs(dist - 5.0) < 0.0001);
    
    assert(npc1->isInAttackRange(npc2) == true);
    
    std::cout << "Distance tests passed!" << std::endl;
}

void testFactory() {
    std::cout << "\nTesting NPC Factory..." << std::endl;
    
    auto knight = NPCFactory::createNPC(NPCType::KNIGHT, "FactoryKnight", 10, 10);
    assert(knight->getType() == NPCType::KNIGHT);
    
    auto squirrel = NPCFactory::createNPC(NPCType::SQUIRREL, "FactorySquirrel", 20, 20);
    assert(squirrel->getType() == NPCType::SQUIRREL);
    
    auto pegasus = NPCFactory::createNPC(NPCType::PEGASUS, "FactoryPegasus", 30, 30);
    assert(pegasus->getType() == NPCType::PEGASUS);
    
    auto randomNPC = NPCFactory::createRandomNPC("Random");
    assert(randomNPC != nullptr);
    
    std::cout << "Factory tests passed!" << std::endl;
}

void testGameEngine() {
    std::cout << "\nTesting Game Engine..." << std::endl;
    
    GameEngine engine;
    engine.initializeGame(10);
    
    auto stats = engine.getStats();
    assert(stats.totalNPCs == 10);
    assert(stats.aliveNPCs == 10);
    assert(stats.deadNPCs == 0);
    
    std::cout << "Game Engine tests passed!" << std::endl;
}

int main() {
    std::cout << "=== Starting Unit Tests ===" << std::endl;
    
    testNPCProperties();
    testMovement();
    testDistance();
    testFactory();
    testGameEngine();
    
    std::cout << "\n=== All tests passed! ===" << std::endl;
    
    return 0;
}