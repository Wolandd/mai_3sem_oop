#include "NPC.h"
#include <sstream>


NPC::NPC(const std::string& characterName, int posX, int posY, NPCType characterType) 
    : characterName(characterName), posX(posX), posY(posY), characterType(characterType), isActive(true) {
    if (posX < 0 || posX > 500 || posY < 0 || posY > 500) {
        throw std::invalid_argument("Coordinates must be in range [0, 500]");
    }
}

std::string NPC::getCharacterName() const { return characterName; }
int NPC::getPositionX() const { return posX; }
int NPC::getPositionY() const { return posY; }
NPCType NPC::getCharacterType() const { return characterType; }
bool NPC::checkIsActive() const { return isActive; }
void NPC::defeat() { isActive = false; }
double NPC::calculateDistance(const std::shared_ptr<NPC>& other) const {
    if (!other) return INFINITY;
    int deltaX = posX - other->getPositionX();
    int deltaY = posY - other->getPositionY();
    return std::sqrt(deltaX * deltaX + deltaY * deltaY);
}

Knight::Knight(const std::string& characterName, int posX, int posY) 
    : NPC(characterName, posX, posY, NPCType::KNIGHT) {}

void Knight::serialize(std::ofstream& file) const {
    file << "KNIGHT " << characterName << " " << posX << " " << posY << std::endl;
}

void Knight::display() const {
    std::cout << "Knight '" << characterName << "' at (" << posX << ", " << posY << ")" 
              << " - " << (isActive ? "Alive" : "Dead") << std::endl;
}

bool Knight::canAttack(const std::shared_ptr<NPC>& attacker) const {
    if (!attacker || !attacker->checkIsActive() || !isActive) {
        return false;
    }    
    if (attacker->getCharacterType() == NPCType::KNIGHT) {
        return false;
    } else if (attacker->getCharacterType() == NPCType::SQUIRREL) {
        return true;
    }
    return false;
}

Squirrel::Squirrel(const std::string& characterName, int posX, int posY) 
    : NPC(characterName, posX, posY, NPCType::SQUIRREL) {}

void Squirrel::serialize(std::ofstream& file) const {
    file << "SQUIRREL " << characterName << " " << posX << " " << posY << std::endl;
}

void Squirrel::display() const {
    std::cout << "Squirrel '" << characterName << "' at (" << posX << ", " << posY << ")" 
              << " - " << (isActive ? "Alive" : "Dead") << std::endl;
}

bool Squirrel::canAttack(const std::shared_ptr<NPC>& attacker) const {
    if (!attacker || !attacker->checkIsActive() || !isActive) {
        return false;
    }
    if (attacker->getCharacterType() == NPCType::SQUIRREL) {
        return false;
    } else if (attacker->getCharacterType() == NPCType::PEGASUS) {
        return true;
    }
    return false;
}

Pegasus::Pegasus(const std::string& characterName, int posX, int posY) 
    : NPC(characterName, posX, posY, NPCType::PEGASUS) {}

void Pegasus::serialize(std::ofstream& file) const {
    file << "PEGASUS " << characterName << " " << posX << " " << posY << std::endl;
}

void Pegasus::display() const {
    std::cout << "Pegasus '" << characterName << "' at (" << posX << ", " << posY << ")" 
              << " - " << (isActive ? "Alive" : "Dead") << std::endl;
}

bool Pegasus::canAttack(const std::shared_ptr<NPC>& attacker) const {
    return false;
}

std::shared_ptr<NPC> NPCFactory::createCharacter(NPCType characterType, const std::string& characterName, int posX, int posY) {
    switch (characterType) {
        case NPCType::KNIGHT:
            return std::make_shared<Knight>(characterName, posX, posY);
        case NPCType::SQUIRREL:
            return std::make_shared<Squirrel>(characterName, posX, posY);
        case NPCType::PEGASUS:
            return std::make_shared<Pegasus>(characterName, posX, posY);
        default:
            throw std::invalid_argument("Unknown character type");
    }
}

std::shared_ptr<NPC> NPCFactory::deserializeFromFile(std::ifstream& file) {
    std::string typeStr, characterName;
    int posX, posY;
    if (!(file >> typeStr >> characterName >> posX >> posY)) {
        return nullptr;
    }
    NPCType characterType;
    if (typeStr == "KNIGHT") {
        characterType = NPCType::KNIGHT;
    } else if (typeStr == "SQUIRREL") {
        characterType = NPCType::SQUIRREL;
    } else if (typeStr == "PEGASUS") {
        characterType = NPCType::PEGASUS;
    } else {
        throw std::runtime_error("Unknown character type in file: " + typeStr);
    }
    return createCharacter(characterType, characterName, posX, posY);
}

void ConsoleObserver::onDefeat(const std::string& attacker, const std::string& target, 
                             NPCType attackerType, NPCType targetType) {
    std::string attackerTypeStr;
    std::string targetTypeStr;
    switch (attackerType) {
        case NPCType::KNIGHT: 
            attackerTypeStr = "Knight"; 
            break;
        case NPCType::SQUIRREL: 
            attackerTypeStr = "Squirrel"; 
            break;
        case NPCType::PEGASUS: 
            attackerTypeStr = "Pegasus"; 
            break;
    }
    switch (targetType) {
        case NPCType::KNIGHT: 
            targetTypeStr = "Knight"; 
            break;
        case NPCType::SQUIRREL: 
            targetTypeStr = "Squirrel"; 
            break;
        case NPCType::PEGASUS: 
            targetTypeStr = "Pegasus"; 
            break;
    }
    
    std::cout << "[BATTLE] " << attackerTypeStr << " '" << attacker 
              << "' killed " << targetTypeStr << " '" << target << "'" << std::endl;
}

FileObserver::FileObserver() {
    logFile.open("log.txt", std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Cannot open log.txt for writing");
    }
}

FileObserver::~FileObserver() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void FileObserver::onDefeat(const std::string& attacker, const std::string& target, 
                          NPCType attackerType, NPCType targetType) {
    if (logFile.is_open()) {
        logFile << attacker << " (type: " << static_cast<int>(attackerType) 
                << ") killed " << target << " (type: " << static_cast<int>(targetType) 
                << ")" << std::endl;
    }
}

void DungeonEditor::addCharacter(const std::shared_ptr<NPC>& character) {
    characters.push_back(character);
}

void DungeonEditor::serializeToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    for (const auto& character : characters) {
        if (character->checkIsActive()) {
            character->serialize(file);
        }
    }
}

void DungeonEditor::deserializeFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }
    characters.clear();
    std::shared_ptr<NPC> character;
    while ((character = NPCFactory::deserializeFromFile(file)) != nullptr) {
        characters.push_back(character);
    }
}

void DungeonEditor::displayAll() const {
    std::cout << "=== All Characters ===" << std::endl;
    for (const auto& character : characters) {
        if (character->checkIsActive()) {
            character->display();
        }
    }
    std::cout << "Total: " << characters.size() << " Characters" << std::endl;
}

void DungeonEditor::addListener(const std::shared_ptr<Observer>& listener) {
    listeners.push_back(listener);
}

void DungeonEditor::notifyDefeat(const std::string& attacker, const std::string& target, 
                               NPCType attackerType, NPCType targetType) {
    for (const auto& listener : listeners) {
        listener->onDefeat(attacker, target, attackerType, targetType);
    }
}

void DungeonEditor::executeCombat(int attackRange) {
    std::cout << "\n=== BATTLE START (range: " << attackRange << ") ===" << std::endl;
    std::vector<std::shared_ptr<NPC>> defeated;
    for (size_t firstIdx = 0; firstIdx < characters.size(); firstIdx++) {
        for (size_t secondIdx = 0; secondIdx < characters.size(); secondIdx++) {
            if (firstIdx == secondIdx) continue;
            auto& firstCharacter = characters[firstIdx];
            auto& secondCharacter = characters[secondIdx];
            if (!firstCharacter->checkIsActive() || !secondCharacter->checkIsActive()) continue;
            double distance = firstCharacter->calculateDistance(secondCharacter);
            if (distance <= attackRange) {
                if (firstCharacter->canAttack(secondCharacter)) {
                    notifyDefeat(firstCharacter->getCharacterName(), secondCharacter->getCharacterName(), 
                                firstCharacter->getCharacterType(), secondCharacter->getCharacterType());
                    secondCharacter->defeat();
                    defeated.push_back(secondCharacter);
                }
                else if (secondCharacter->canAttack(firstCharacter)) {
                    notifyDefeat(secondCharacter->getCharacterName(), firstCharacter->getCharacterName(), 
                                secondCharacter->getCharacterType(), firstCharacter->getCharacterType());
                    firstCharacter->defeat();
                    defeated.push_back(firstCharacter);
                }
            }
        }
    }

    characters.erase(std::remove_if(characters.begin(), characters.end(), 
               [](const std::shared_ptr<NPC>& character) { return !character->checkIsActive(); }), 
               characters.end());
    std::cout << "=== BATTLE END ===" << std::endl;
    std::cout << "Killed: " << defeated.size() << " Characters" << std::endl;
    std::cout << "Survivors: " << characters.size() << " Characters" << std::endl;
}

}