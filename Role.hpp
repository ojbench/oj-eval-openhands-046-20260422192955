#include<string>
#include<iostream>
#ifndef ROLE_HPP
#define ROLE_HPP
namespace final{

    class Fighter;
    class Protector;
    class Caster;

    class Base{
        friend class Fighter;
        friend class Protector;
        friend class Caster;
    private:
        std::string name;
        int health,attack;
        bool alive,defensive;
        inline static int aliveNumber = 0; // total alive count
    protected:
        Base(const std::string& n, const int h, const int a):name(n),health(h),attack(a),alive(true),defensive(false){
            ++aliveNumber;
        }
    public:
        virtual ~Base() = default;
        virtual void launch_attack(Base* target) = 0;
        virtual void defend(bool flag) = 0;
        static int getAliveNumber(){
            return aliveNumber;
        }
        bool isAlive(){
            return alive;
        }
        bool isDefensive(){
            return defensive;
        }
    };

    class Fighter:public Base{
    public:
        Fighter (const std::string& name, const int health,const int attack):Base(name,health,attack){}

        void launch_attack(Base* target);

        void defend(bool flag){
            if (flag) {
                defensive = 1;
                health += 3;
                attack += 2;
            } else {
                defensive = 0;
            }
        }
    };

    class Protector:public Base{
    public:
        Protector (const std::string& name, const int health,const int attack):Base(name,health,attack){}

        void launch_attack(Base* target);

        void defend(bool flag){
            if (flag) {
                defensive = 1;
                health += 7;
                attack += 1;
            } else {
                defensive = 0;
            }
        }
    };

    class Caster:public Base{
    public:
        Caster (const std::string& name, const int health,const int attack):Base(name,health,attack){}

        void launch_attack(Base* target);

        void defend(bool flag){
            if (flag) {
                defensive = 1;
                health += 5;
            } else {
                defensive = 0;
            }
        }
    };


    static inline std::string role_name(const Base* p){
        if (dynamic_cast<const Fighter*>(p)) return "Fighter";
        if (dynamic_cast<const Protector*>(p)) return "Protector";
        return "Caster";
    }



    void Fighter::launch_attack(Base *target) {
        if (!alive) return;
        int factor = dynamic_cast<Caster*>(target) ? 2 : 1;
        int dmg_to_target = attack * factor;
        int self_damage = 0;
        if (target->defensive){
            int effective = dmg_to_target - target->attack;
            if (effective < 0) effective = 0;
            target->health -= effective;
            // retaliation (not for Caster attacker)
            self_damage = target->attack;
            health -= self_damage;
        } else {
            target->health -= dmg_to_target;
        }
        bool target_just_died = false;
        if (target->alive && target->health <= 0){
            target->alive = false;
            --aliveNumber;
            std::cout << role_name(target) << ' ' << target->name << " is killed\n";
            target_just_died = true;
        }
        if (alive && health <= 0){
            alive = false;
            --aliveNumber;
            std::cout << "Fighter " << name << " is killed\n";
        }
    }

    void Protector::launch_attack(Base *target) {
        if (!alive) return;
        int factor = dynamic_cast<Fighter*>(target) ? 2 : 1;
        int dmg_to_target = attack * factor;
        if (target->defensive){
            int effective = dmg_to_target - target->attack;
            if (effective < 0) effective = 0;
            target->health -= effective;
            // retaliation (not for Caster attacker, but we're Protector)
            health -= target->attack;
        } else {
            target->health -= dmg_to_target;
        }
        if (target->alive && target->health <= 0){
            target->alive = false;
            --aliveNumber;
            std::cout << role_name(target) << ' ' << target->name << " is killed\n";
        }
        if (alive && health <= 0){
            alive = false;
            --aliveNumber;
            std::cout << "Protector " << name << " is killed\n";
        }
    }

    void Caster::launch_attack(Base *target) {
        if (!alive) return;
        int factor = dynamic_cast<Protector*>(target) ? 2 : 1;
        int dmg_to_target = attack * factor;
        if (target->defensive){
            int effective = dmg_to_target - target->attack;
            if (effective < 0) effective = 0;
            target->health -= effective;
            // retaliation: caster takes no retaliation
        } else {
            target->health -= dmg_to_target;
        }
        if (target->alive && target->health <= 0){
            target->alive = false;
            --aliveNumber;
            std::cout << role_name(target) << ' ' << target->name << " is killed\n";
        }
        if (alive && health <= 0){
            alive = false;
            --aliveNumber;
            std::cout << "Caster " << name << " is killed\n";
        }
    }
}
#endif //ROLE_HPP
