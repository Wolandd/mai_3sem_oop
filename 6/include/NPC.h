#ifndef NPC_H
#define NPC_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <cmath>
#include <algorithm>


enum class NPCType {
    KNIGHT,
    SQUIRREL,
    PEGASUS
};

class NPC {
protected:
    std::string characterName;
    int posX, posY;
    NPCType characterType;
    bool isActive;

public:
    NPC(const std::string& characterName, int posX, int posY, NPCType characterType);
    virtual ~NPC() = default;
    std::string getCharacterName() const;
    int getPositionX() const;
    int getPositionY() const;
    NPCType getCharacterType() const;
    bool checkIsActive() const;
    void defeat();
    virtual void serialize(std::ofstream& file) const = 0;
    virtual void display() const = 0;
    virtual bool canAttack(const std::shared_ptr<NPC>& attacker) const = 0;
    double calculateDistance(const std::shared_ptr<NPC>& other) const;
};

class Knight : public NPC {
public:
    Knight(const std::string& characterName, int posX, int posY);
    void serialize(std::ofstream& file) const override;
    void display() const override;
    bool canAttack(const std::shared_ptr<NPC>& attacker) const override;
};

class Squirrel : public NPC {
public:
    Squirrel(const std::string& characterName, int posX, int posY);
    void serialize(std::ofstream& file) const override;
    void display() const override;
    bool canAttack(const std::shared_ptr<NPC>& attacker) const override;
};

class Pegasus : public NPC {
public:
    Pegasus(const std::string& characterName, int posX, int posY);
    void serialize(std::ofstream& file) const override;
    void display() const override;
    bool canAttack(const std::shared_ptr<NPC>& attacker) const override;
};

class NPCFactory {
public:
    static std::shared_ptr<NPC> createCharacter(NPCType characterType, const std::string& characterName, int posX, int posY);
    static std::shared_ptr<NPC> deserializeFromFile(std::ifstream& file);
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onDefeat(const std::string& attacker, const std::string& target, NPCType attackerType, NPCType targetType) = 0;
};

class ConsoleObserver : public Observer {
public:
    void onDefeat(const std::string& attacker, const std::string& target, NPCType attackerType, NPCType targetType) override;
};

class FileObserver : public Observer {
private:
    std::ofstream logFile;
public:
    FileObserver();
    ~FileObserver();
    void onDefeat(const std::string& attacker, const std::string& target, NPCType attackerType, NPCType targetType) override;
};

class DungeonEditor {
private:
    std::vector<std::shared_ptr<NPC>> characters;
    std::vector<std::shared_ptr<Observer>> listeners;
public:
    void addCharacter(const std::shared_ptr<NPC>& character);
    void serializeToFile(const std::string& filename) const;
    void deserializeFromFile(const std::string& filename);
    void displayAll() const;
    void addListener(const std::shared_ptr<Observer>& listener);
    void notifyDefeat(const std::string& attacker, const std::string& target, NPCType attackerType, NPCType targetType);
    void executeCombat(int attackRange);
};

#endif