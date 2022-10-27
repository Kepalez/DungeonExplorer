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
    string name;
    public:
    Character(){}
    Character(string name,int Health,int Damage){
        this->name = name;
        this->TotalHealth = Health;
        this->baseDamage = Damage;
        currentHealt = TotalHealth;
        defense = 0;
    }
    ~Character(){}    virtual void Die(){}
    void recieveDmg(int dmg){
        dmg-=defense;
        if(dmg < 0) dmg = 0;
        currentHealt-=dmg;
        if(currentHealt <= 0) Die();
    }
    void dealDamage(Character target){
        damageToDeal = baseDamage;
        target.recieveDmg(damageToDeal);
    }
    void setPosition(int row,int column){
        rowPos = row;
        colPos = column;
    }
    void move(int row,int column){
        rowPos+=row;
        colPos+=column;
    }
    int getRow(){return rowPos;}
    int getColumn(){return colPos;}
};

class Hero: public Character{
    private: 
    bool Dead;
    char body = 'P';
    public:
    Hero(string name,int Health,int Damage): Character(name, Health, Damage){Dead = false;}
    ~Hero(){}
    void Die(){
        Dead = true;
    }
    bool isDead(){return Dead;}
    char getBody(){return body;}
};

class Monster: public Character{
    private:
    char body = 'M';
    bool Dead;
    public:
    Monster(): Character(){}
    Monster(string name,int health, int damage): Character(name,health,damage){Dead = false;}
    ~Monster(){}
    char getBody(){return body;}
    void Die(){Dead = true;}
    bool isDead(){return Dead;}
};