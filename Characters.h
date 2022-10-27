#include "Constants.h"

class Character{
    private:
    int TotalHealth;
    int currentHealt;
    int rowPos;
    int colPos;
    int defense;
    int baseDamage;
    int damageToDeal;
    bool Dead;
    string name;
    public:
    Character(){}
    Character(string name,int Health,int Damage){
        Dead = false;
        this->name = name;
        this->TotalHealth = Health;
        this->baseDamage = Damage;
        currentHealt = TotalHealth;
        defense = 0;
    }
    ~Character(){}    
    void recieveDmg(int dmg){
        dmg-=defense;
        if(dmg < 0) dmg = 0;
        currentHealt-=dmg;
        if(currentHealt <= 0) Dead = true;
    }
    void dealDamage(Character target){
        damageToDeal = baseDamage;
        target.recieveDmg(damageToDeal);
        recieveDmg(target.getDMG());
    }
    void setPosition(int row,int column){
        rowPos = row;
        colPos = column;
    }
    void move(int row,int column){
        rowPos+=row;
        colPos+=column;
    }
    bool isDead(){return Dead;}
    int getRow(){return rowPos;}
    int getColumn(){return colPos;}
    int getRemainingHP(){return currentHealt;}
    int getDMG(){return baseDamage;}
};

class Hero: public Character{
    private: 
    char body;
    string name;
    public:
    Hero(){}
    Hero(string name,int Health,int Damage): Character(name, Health, Damage){
        this->name = name;
        body = name[0];
    }
    ~Hero(){}
    char getBody(){return body;}
    string getName(){return name;}

    void setName(string cad){name = cad;}
};

class Monster: public Character{
    private:
    pair<int,int> lastSeen;
    char body = 'M';
    string status;
    public:
    Monster(): Character(){}
    Monster(string name,int health, int damage): Character(name,health,damage){
        status = "Idle";  
    }
    ~Monster(){}
    void setStatus(string s){status = s;}
    void setLastSeen(int row, int col){lastSeen.first = row;lastSeen.second = col;}
    char getBody(){return body;}
    string getStatus(){return status;}
    pair <int,int> getLastSeen(){return lastSeen;}
};