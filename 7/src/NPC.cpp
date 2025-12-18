#include "NPC.h"
#include <sstream>
#include <random>
#include <chrono>
#include <map>
#include <iomanip>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dice(1, 6);

NPC::NPC(const std::string& identifier, int posX, int posY, NPCType type, int moveRange, int attackRange) 
    : identifier(identifier), posX(posX), posY(posY), type(type), isActive(true), 
      moveRange(moveRange), attackRange(attackRange) {
    if (posX < 0 || posX >= FIELD_WIDTH || posY < 0 || posY >= FIELD_HEIGHT) {
        throw std::invalid_argument("Coordinates out of map bounds");
    }
}

std::string NPC::getIdentifier() const { return identifier; }
int NPC::getPositionX() const { return posX; }
int NPC::getPositionY() const { return posY; }
NPCType NPC::getType() const { return type; }
bool NPC::isActiveNPC() const { return isActive; }
void NPC::setDead() { isActive = false; }
int NPC::getMoveRange() const { return moveRange; }
int NPC::getAttackRange() const { return attackRange; }

void NPC::performMove() {
    if (!isActive) return;
    
    std::uniform_int_distribution<int> moveDist(0, moveRange);
    std::uniform_int_distribution<int> directionDist(0, 3);
    
    int distance = moveDist(gen);
    int direction = directionDist(gen);
    
    int newX = posX;
    int newY = posY;
    
    switch (direction) {
        case 0: newY -= distance; break;
        case 1: newX += distance; break;
        case 2: newY += distance; break;
        case 3: newX -= distance; break;
    }
    
    newX = std::max(0, std::min(FIELD_WIDTH - 1, newX));
    newY = std::max(0, std::min(FIELD_HEIGHT - 1, newY));
    
    posX = newX;
    posY = newY;
}

double NPC::calculateDistance(const std::shared_ptr<NPC>& other) const {
    if (!other || !other->isActiveNPC()) return INFINITY;
    int dx = posX - other->getPositionX();
    int dy = posY - other->getPositionY();
    return std::sqrt(dx*dx + dy*dy);
}

bool NPC::isInAttackRange(const std::shared_ptr<NPC>& other) const {
    return calculateDistance(other) <= attackRange;
}

int NPC::generateAttackRoll() const {
    return dice(gen);
}

int NPC::generateDefenseRoll() const {
    return dice(gen);
}

Knight::Knight(const std::string& name, int x, int y) 
    : NPC(name, x, y, NPCType::KNIGHT, 30, 10) {}

void Knight::save(std::ofstream& file) const {
    file << "KNIGHT " << identifier << " " << posX << " " << posY << std::endl;
}

void Knight::print() const {
    std::cout << "Knight '" << identifier << "' at (" << posX << ", " << posY << ")" 
              << " - " << (isActive ? "Alive" : "Dead") 
              << " [Move: " << moveRange << ", Kill: " << attackRange << "]" << std::endl;
}

char Knight::getSymbol() const {
    return 'K';
}

bool Knight::canAttack(NPCType targetType) const {
    return targetType == NPCType::SQUIRREL;
}

Squirrel::Squirrel(const std::string& name, int x, int y) 
    : NPC(name, x, y, NPCType::SQUIRREL, 5, 10) {}

void Squirrel::save(std::ofstream& file) const {
    file << "SQUIRREL " << identifier << " " << posX << " " << posY << std::endl;
}

void Squirrel::print() const {
    std::cout << "Squirrel '" << identifier << "' at (" << posX << ", " << posY << ")" 
              << " - " << (isActive ? "Alive" : "Dead") 
              << " [Move: " << moveRange << ", Kill: " << attackRange << "]" << std::endl;
}

char Squirrel::getSymbol() const {
    return 'S';
}

bool Squirrel::canAttack(NPCType targetType) const {
    return targetType == NPCType::PEGASUS;
}

Pegasus::Pegasus(const std::string& name, int x, int y) 
    : NPC(name, x, y, NPCType::PEGASUS, 30, 10) {}

void Pegasus::save(std::ofstream& file) const {
    file << "PEGASUS " << identifier << " " << posX << " " << posY << std::endl;
}

void Pegasus::print() const {
    std::cout << "Pegasus '" << identifier << "' at (" << posX << ", " << posY << ")" 
              << " - " << (isActive ? "Alive" : "Dead") 
              << " [Move: " << moveRange << ", Kill: " << attackRange << "]" << std::endl;
}

char Pegasus::getSymbol() const {
    return 'P';
}

bool Pegasus::canAttack(NPCType targetType) const {
    return false;
}

std::shared_ptr<NPC> NPCFactory::createNPC(NPCType type, const std::string& name, int x, int y) {
    switch (type) {
        case NPCType::KNIGHT:
            return std::make_shared<Knight>(name, x, y);
        case NPCType::SQUIRREL:
            return std::make_shared<Squirrel>(name, x, y);
        case NPCType::PEGASUS:
            return std::make_shared<Pegasus>(name, x, y);
        default:
            throw std::invalid_argument("Unknown NPC type");
    }
}

std::shared_ptr<NPC> NPCFactory::createRandomNPC(const std::string& name) {
    std::uniform_int_distribution<int> typeDist(0, 2);
    std::uniform_int_distribution<int> coordX(0, FIELD_WIDTH - 1);
    std::uniform_int_distribution<int> coordY(0, FIELD_HEIGHT - 1);
    
    NPCType type = static_cast<NPCType>(typeDist(gen));
    int x = coordX(gen);
    int y = coordY(gen);
    
    return createNPC(type, name, x, y);
}

std::shared_ptr<NPC> NPCFactory::loadFromFile(std::ifstream& file) {
    std::string typeStr, name;
    int x, y;
    
    if (!(file >> typeStr >> name >> x >> y)) {
        return nullptr;
    }
    
    NPCType type;
    if (typeStr == "KNIGHT") type = NPCType::KNIGHT;
    else if (typeStr == "SQUIRREL") type = NPCType::SQUIRREL;
    else if (typeStr == "PEGASUS") type = NPCType::PEGASUS;
    else throw std::runtime_error("Unknown NPC type in file: " + typeStr);
    
    return createNPC(type, name, x, y);
}

void ConsoleObserver::onKill(const std::string& killer, const std::string& victim, 
                            NPCType killerType, NPCType victimType) {
    std::lock_guard<std::mutex> lock(outputMutex);
    
    std::string killerTypeStr, victimTypeStr;
    
    switch (killerType) {
        case NPCType::KNIGHT: killerTypeStr = "Knight"; break;
        case NPCType::SQUIRREL: killerTypeStr = "Squirrel"; break;
        case NPCType::PEGASUS: killerTypeStr = "Pegasus"; break;
    }
    
    switch (victimType) {
        case NPCType::KNIGHT: victimTypeStr = "Knight"; break;
        case NPCType::SQUIRREL: victimTypeStr = "Squirrel"; break;
        case NPCType::PEGASUS: victimTypeStr = "Pegasus"; break;
    }
    
    std::cout << "[BATTLE] " << killerTypeStr << " '" << killer 
              << "' killed " << victimTypeStr << " '" << victim << "'" << std::endl;
}

void ConsoleObserver::onGameStart() {
    std::lock_guard<std::mutex> lock(outputMutex);
    std::cout << "=== GAME STARTED ===" << std::endl;
    std::cout << "Map size: " << FIELD_WIDTH << "x" << FIELD_HEIGHT << std::endl;
    std::cout << "Game duration: " << ROUND_DURATION << " seconds" << std::endl;
}

void ConsoleObserver::onGameEnd(const GameStatistics& stats) {
    std::lock_guard<std::mutex> lock(outputMutex);
    std::cout << "\n=== GAME ENDED ===" << std::endl;
    std::cout << "Total NPCs: " << stats.totalNPCs << std::endl;
    std::cout << "Survivors: " << stats.aliveNPCs << std::endl;
    std::cout << "Killed: " << stats.deadNPCs << std::endl;
    
    std::cout << "\nSurvivors by type:" << std::endl;
    for (const auto& [type, count] : stats.byType) {
        std::string typeStr;
        switch (type) {
            case NPCType::KNIGHT: typeStr = "Knights"; break;
            case NPCType::SQUIRREL: typeStr = "Squirrels"; break;
            case NPCType::PEGASUS: typeStr = "Pegasuses"; break;
        }
        std::cout << "  " << typeStr << ": " << count << std::endl;
    }
    
    if (!stats.kills.empty()) {
        std::cout << "\nTop killers:" << std::endl;
        for (const auto& [name, kills] : stats.kills) {
            std::cout << "  " << name << ": " << kills << " kills" << std::endl;
        }
    }
}

FileObserver::FileObserver() {
    logStream.open("log.txt", std::ios::app);
    if (!logStream.is_open()) {
        throw std::runtime_error("Cannot open log.txt for writing");
    }
}

FileObserver::~FileObserver() {
    if (logStream.is_open()) {
        logStream.close();
    }
}

void FileObserver::onKill(const std::string& killer, const std::string& victim, 
                         NPCType killerType, NPCType victimType) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logStream.is_open()) {
        logStream << "[KILL] " << killer << " (type: " << static_cast<int>(killerType) 
                << ") killed " << victim << " (type: " << static_cast<int>(victimType) 
                << ")" << std::endl;
    }
}

void FileObserver::onGameStart() {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logStream.is_open()) {
        logStream << "\n=== NEW GAME STARTED ===" << std::endl;
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        logStream << "Timestamp: " << std::ctime(&time);
    }
}

void FileObserver::onGameEnd(const GameStatistics& stats) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logStream.is_open()) {
        logStream << "\n=== GAME ENDED ===" << std::endl;
        logStream << "Total NPCs: " << stats.totalNPCs << std::endl;
        logStream << "Survivors: " << stats.aliveNPCs << std::endl;
        logStream << "Killed: " << stats.deadNPCs << std::endl;
        
        for (const auto& [name, kills] : stats.kills) {
            logStream << "Killer " << name << ": " << kills << " kills" << std::endl;
        }
    }
}

GameEngine::GameEngine() : isRunning(false), isGameOver(false) {}

GameEngine::~GameEngine() {
    stop();
}

void GameEngine::initializeGame(int npcCount) {
    createRandomNPCs(npcCount);
    updateStats();
}

void GameEngine::createRandomNPCs(int count) {
    std::lock_guard<std::mutex> lock(npcsMutex);
    npcs.clear();
    
    for (int i = 0; i < count; ++i) {
        std::string name = "NPC_" + std::to_string(i);
        auto npc = NPCFactory::createRandomNPC(name);
        npcs.push_back(npc);
    }
}

void GameEngine::addObserver(const std::shared_ptr<Observer>& observer) {
    observers.push_back(observer);
}

void GameEngine::notifyKill(const std::string& killer, const std::string& victim, 
                           NPCType killerType, NPCType victimType) {
    for (const auto& observer : observers) {
        observer->onKill(killer, victim, killerType, victimType);
    }
    
    {
        std::lock_guard<std::mutex> lock(statsMutex);
        killCount[killer]++;
    }
}

void GameEngine::movementLoop() {
    while (isRunning && !isGameOver) {
        {
            std::lock_guard<std::mutex> lock(npcsMutex);
            
            for (auto& npc : npcs) {
                if (npc->isActiveNPC()) {
                    npc->performMove();
                }
            }
            
            for (size_t i = 0; i < npcs.size(); ++i) {
                if (!npcs[i]->isActiveNPC()) continue;
                
                for (size_t j = i + 1; j < npcs.size(); ++j) {
                    if (!npcs[j]->isActiveNPC()) continue;
                    
                    if (npcs[i]->isInAttackRange(npcs[j])) {
                        if (npcs[i]->canAttack(npcs[j]->getType())) {
                            {
                                std::lock_guard<std::mutex> combatLock(combatQueueMutex);
                                combatQueue.push(CombatTask(npcs[i], npcs[j]));
                                combatCV.notify_one();
                            }
                        }
                        else if (npcs[j]->canAttack(npcs[i]->getType())) {
                            {
                                std::lock_guard<std::mutex> combatLock(combatQueueMutex);
                                combatQueue.push(CombatTask(npcs[j], npcs[i]));
                                combatCV.notify_one();
                            }
                        }
                    }
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void GameEngine::combatLoop() {
    while (isRunning && !isGameOver) {
        CombatTask task(nullptr, nullptr);
        bool hasTask = false;
        
        {
            std::unique_lock<std::mutex> lock(combatQueueMutex);
            
            combatCV.wait_for(lock, std::chrono::milliseconds(100), 
                            [this]() { return !combatQueue.empty() || !isRunning; });
            
            if (!combatQueue.empty()) {
                task = combatQueue.front();
                combatQueue.pop();
                hasTask = true;
            }
        }
        
        if (hasTask && task.attacker && task.defender) {
            processBattle(task);
        }
    }
}

void GameEngine::processBattle(CombatTask& task) {
    if (!task.attacker->isActiveNPC() || !task.defender->isActiveNPC()) {
        return;
    }
    
    int attackRoll = task.attacker->generateAttackRoll();
    int defenseRoll = task.defender->generateDefenseRoll();
    
    {
        std::lock_guard<std::mutex> lock(npcsMutex);
        
        if (!task.attacker->isActiveNPC() || !task.defender->isActiveNPC()) {
            return;
        }
        
        if (attackRoll > defenseRoll) {
            task.defender->setDead();
            notifyKill(task.attacker->getIdentifier(), task.defender->getIdentifier(),
                      task.attacker->getType(), task.defender->getType());
            
            cleanupDeadNPCs();
        }
    }
}

void GameEngine::cleanupDeadNPCs() {
    npcs.erase(std::remove_if(npcs.begin(), npcs.end(),
               [](const std::shared_ptr<NPC>& npc) { return !npc->isActiveNPC(); }),
               npcs.end());
    updateStats();
}

void GameEngine::displayLoop() {
    auto startTime = std::chrono::steady_clock::now();
    
    while (isRunning && !isGameOver) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        
        if (elapsed >= ROUND_DURATION) {
            isGameOver = true;
            break;
        }
        
        printMap(elapsed);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    isGameOver = true;
    combatCV.notify_all();
}

void GameEngine::printMap(int elapsedSeconds) {
    std::lock_guard<std::mutex> lock(npcsMutex);
    
    std::cout << "\033[2J\033[1;1H";
    
    std::cout << "=== BALAGUR FATE 3 - DUNGEON BATTLE ===" << std::endl;
    std::cout << "Time: " << elapsedSeconds << "/" << ROUND_DURATION << "s | ";
    auto stats = getStats();
    std::cout << "Alive: " << stats.aliveNPCs << " | Dead: " << stats.deadNPCs << std::endl;
    std::cout << "=========================================" << std::endl;
    
    const int DISPLAY_SIZE = 25;
    char map[DISPLAY_SIZE][DISPLAY_SIZE];
    
    for (int i = 0; i < DISPLAY_SIZE; ++i) {
        for (int j = 0; j < DISPLAY_SIZE; ++j) {
            map[i][j] = '.';
        }
    }
    
    int visibleNPCs = 0;
    for (const auto& npc : npcs) {
        if (npc->isActiveNPC()) {
            int x = npc->getPositionX();
            int y = npc->getPositionY();
            if (x >= 0 && x < DISPLAY_SIZE && y >= 0 && y < DISPLAY_SIZE) {
                map[y][x] = npc->getSymbol();
                visibleNPCs++;
            }
        }
    }
    
    std::cout << "   ";
    for (int j = 0; j < DISPLAY_SIZE; ++j) {
        std::cout << std::setw(2) << j % 10;
    }
    std::cout << std::endl;
    
    std::cout << "  +";
    for (int j = 0; j < DISPLAY_SIZE; ++j) {
        std::cout << "--";
    }
    std::cout << "+" << std::endl;
    
    for (int i = 0; i < DISPLAY_SIZE; ++i) {
        std::cout << std::setw(2) << i % 10 << "|";
        for (int j = 0; j < DISPLAY_SIZE; ++j) {
            std::cout << map[i][j] << " ";
        }
        std::cout << "|" << std::endl;
    }
    
    std::cout << "  +";
    for (int j = 0; j < DISPLAY_SIZE; ++j) {
        std::cout << "--";
    }
    std::cout << "+" << std::endl;
    
    std::cout << "\nLEGEND: K = Knight (Move:30, Kill:10), S = Squirrel (Move:5, Kill:10), P = Pegasus (Move:30, Kill:10)" << std::endl;
    std::cout << "Visible on map: " << visibleNPCs << " of " << stats.aliveNPCs << " alive NPCs" << std::endl;
    std::cout << "Time remaining: " << (ROUND_DURATION - elapsedSeconds) << " seconds" << std::endl;
    
    if (!stats.byType.empty()) {
        std::cout << "Alive by type: ";
        bool first = true;
        for (const auto& [type, count] : stats.byType) {
            if (!first) std::cout << ", ";
            switch (type) {
                case NPCType::KNIGHT: std::cout << "Knights: " << count; break;
                case NPCType::SQUIRREL: std::cout << "Squirrels: " << count; break;
                case NPCType::PEGASUS: std::cout << "Pegasuses: " << count; break;
            }
            first = false;
        }
        std::cout << std::endl;
    }
}

GameStatistics GameEngine::getStats() {
    std::lock_guard<std::mutex> lock(statsMutex);
    return statistics;
}

void GameEngine::updateStats() {
    std::lock_guard<std::mutex> lock(statsMutex);
    
    statistics.totalNPCs = npcs.size();
    statistics.aliveNPCs = 0;
    statistics.deadNPCs = 0;
    statistics.byType.clear();
    statistics.kills = killCount;
    
    for (const auto& npc : npcs) {
        if (npc->isActiveNPC()) {
            statistics.aliveNPCs++;
            statistics.byType[npc->getType()]++;
        } else {
            statistics.deadNPCs++;
        }
    }
}

void GameEngine::run() {
    for (const auto& observer : observers) {
        observer->onGameStart();
    }
    
    isRunning = true;
    isGameOver = false;
    
    movementThread = std::thread(&GameEngine::movementLoop, this);
    combatThread = std::thread(&GameEngine::combatLoop, this);
    displayThread = std::thread(&GameEngine::displayLoop, this);
    
    displayThread.join();
    
    stop();
    
    updateStats();
    for (const auto& observer : observers) {
        observer->onGameEnd(getStats());
    }
    
    {
        std::lock_guard<std::mutex> lock(npcsMutex);
        std::cout << "\n=== SURVIVORS ===" << std::endl;
        for (const auto& npc : npcs) {
            if (npc->isActiveNPC()) {
                npc->print();
            }
        }
        if (npcs.empty()) {
            std::cout << "No survivors!" << std::endl;
        }
    }
}

void GameEngine::stop() {
    isRunning = false;
    isGameOver = true;
    combatCV.notify_all();
    
    if (movementThread.joinable()) movementThread.join();
    if (combatThread.joinable()) combatThread.join();
    if (displayThread.joinable()) displayThread.join();
}