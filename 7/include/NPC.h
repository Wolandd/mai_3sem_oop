#ifndef NPC_H
#define NPC_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <random>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <chrono>
#include <condition_variable>
#include <map>

const int FIELD_WIDTH = 100;
const int FIELD_HEIGHT = 100;
const int ROUND_DURATION = 30;

enum class NPCType {
    KNIGHT,
    SQUIRREL,
    PEGASUS
};

struct GameStatistics {
    int totalNPCs = 0;
    int aliveNPCs = 0;
    int deadNPCs = 0;
    std::map<NPCType, int> byType;
    std::map<std::string, int> kills;
};

class NPC {
protected:
    std::string identifier;
    int posX, posY;
    NPCType type;
    bool isActive;
    int moveRange;
    int attackRange;
    
public:
    NPC(const std::string& identifier, int posX, int posY, NPCType type, int moveRange, int attackRange);
    virtual ~NPC() = default;

    std::string getIdentifier() const;
    int getPositionX() const;
    int getPositionY() const;
    NPCType getType() const;
    bool isActiveNPC() const;
    void setDead();
    int getMoveRange() const;
    int getAttackRange() const;

    virtual void save(std::ofstream& file) const = 0;
    virtual void print() const = 0;
    virtual char getSymbol() const = 0;
    
    virtual bool canAttack(NPCType targetType) const = 0;
    
    void performMove();
    
    double calculateDistance(const std::shared_ptr<NPC>& other) const;
    bool isInAttackRange(const std::shared_ptr<NPC>& other) const;
    
    virtual int generateAttackRoll() const;
    virtual int generateDefenseRoll() const;
};

class Knight : public NPC {
public:
    Knight(const std::string& name, int x, int y);
    void save(std::ofstream& file) const override;
    void print() const override;
    char getSymbol() const override;
    bool canAttack(NPCType targetType) const override;
};

class Squirrel : public NPC {
public:
    Squirrel(const std::string& name, int x, int y);
    void save(std::ofstream& file) const override;
    void print() const override;
    char getSymbol() const override;
    bool canAttack(NPCType targetType) const override;
};

class Pegasus : public NPC {
public:
    Pegasus(const std::string& name, int x, int y);
    void save(std::ofstream& file) const override;
    void print() const override;
    char getSymbol() const override;
    bool canAttack(NPCType targetType) const override;
};

class NPCFactory {
public:
    static std::shared_ptr<NPC> createNPC(NPCType type, const std::string& name, int x, int y);
    static std::shared_ptr<NPC> loadFromFile(std::ifstream& file);
    static std::shared_ptr<NPC> createRandomNPC(const std::string& name);
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onKill(const std::string& killer, const std::string& victim, 
                       NPCType killerType, NPCType victimType) = 0;
    virtual void onGameStart() = 0;
    virtual void onGameEnd(const GameStatistics& stats) = 0;
};

class ConsoleObserver : public Observer {
private:
    mutable std::mutex outputMutex;
public:
    void onKill(const std::string& killer, const std::string& victim, 
               NPCType killerType, NPCType victimType) override;
    void onGameStart() override;
    void onGameEnd(const GameStatistics& stats) override;
};

class FileObserver : public Observer {
private:
    std::ofstream logStream;
    mutable std::mutex logMutex;
public:
    FileObserver();
    ~FileObserver();
    void onKill(const std::string& killer, const std::string& victim, 
               NPCType killerType, NPCType victimType) override;
    void onGameStart() override;
    void onGameEnd(const GameStatistics& stats) override;
};

struct CombatTask {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
    
    CombatTask(std::shared_ptr<NPC> a, std::shared_ptr<NPC> d) 
        : attacker(a), defender(d) {}
};

class GameEngine {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    std::vector<std::shared_ptr<Observer>> observers;
    
    std::thread movementThread;
    std::thread combatThread;
    std::thread displayThread;
    
    std::queue<CombatTask> combatQueue;
    
    mutable std::mutex npcsMutex;
    mutable std::mutex combatQueueMutex;
    mutable std::mutex statsMutex;
    
    std::condition_variable combatCV;
    
    std::atomic<bool> isRunning;
    std::atomic<bool> isGameOver;
    
    GameStatistics statistics;
    std::map<std::string, int> killCount;
    
public:
    GameEngine();
    ~GameEngine();
    
    void initializeGame(int npcCount = 50);
    void addObserver(const std::shared_ptr<Observer>& observer);
    void notifyKill(const std::string& killer, const std::string& victim, 
                   NPCType killerType, NPCType victimType);
    
    void run();
    void stop();
    
    void movementLoop();
    void combatLoop();
    void displayLoop();
    
    void processBattle(CombatTask& task);
    void printMap(int elapsedSeconds);
    GameStatistics getStats();
    void updateStats();
    
private:
    void createRandomNPCs(int count);
    void cleanupDeadNPCs();
};

#endif