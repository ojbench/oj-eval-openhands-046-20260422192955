#include <bits/stdc++.h>
using namespace std;
namespace final{
    class Fighter; class Protector; class Caster;
    class Base{
        friend class Fighter; friend class Protector; friend class Caster;
    private:
        string name; int health, attack; bool alive, defensive;
        static int aliveNumber;
    protected:
        Base(const string& n, const int h, const int a):name(n),health(h),attack(a),alive(true),defensive(false){ ++aliveNumber; }
    public:
        virtual ~Base() = default;
        static int getAliveNumber(){ return aliveNumber; }
        bool isAlive(){ return alive; }
        bool isDefensive(){ return defensive; }
        virtual void launch_attack(Base* target)=0;
        virtual void defend(bool flag)=0;
    };
    int Base::aliveNumber = 0;

    class Fighter: public Base{
    public:
        Fighter(const string& name, const int health, const int attack):Base(name,health,attack){}
        void launch_attack(Base* target);
        void defend(bool flag){ if(flag){ defensive=1; health+=3; attack+=2; } else defensive=0; }
    };
    class Protector: public Base{
    public:
        Protector(const string& name, const int health, const int attack):Base(name,health,attack){}
        void launch_attack(Base* target);
        void defend(bool flag){ if(flag){ defensive=1; health+=7; attack+=1; } else defensive=0; }
    };
    class Caster: public Base{
    public:
        Caster(const string& name, const int health, const int attack):Base(name,health,attack){}
        void launch_attack(Base* target);
        void defend(bool flag){ if(flag){ defensive=1; health+=5; } else defensive=0; }
    };
    static inline string role_name(const Base* p){
        if(dynamic_cast<const Fighter*>(p)) return "Fighter";
        if(dynamic_cast<const Protector*>(p)) return "Protector";
        return "Caster";
    }
    void Fighter::launch_attack(Base* target){
        if(!alive) return;
        int factor = dynamic_cast<Caster*>(target)?2:1;
        int dmg = attack*factor;
        if(target->defensive){
            int effective = dmg - target->attack; if(effective<0) effective=0; target->health -= effective;
            health -= target->attack; // retaliation since attacker not Caster
        }else{
            target->health -= dmg;
        }
        if(target->alive && target->health<=0){ target->alive=false; --Base::aliveNumber; cout<<role_name(target)<<' '<<target->name<<" is killed\n"; }
        if(alive && health<=0){ alive=false; --Base::aliveNumber; cout<<"Fighter "<<name<<" is killed\n"; }
    }
    void Protector::launch_attack(Base* target){
        if(!alive) return;
        int factor = dynamic_cast<Fighter*>(target)?2:1;
        int dmg = attack*factor;
        if(target->defensive){ int effective = dmg - target->attack; if(effective<0) effective=0; target->health -= effective; health -= target->attack; }
        else { target->health -= dmg; }
        if(target->alive && target->health<=0){ target->alive=false; --Base::aliveNumber; cout<<role_name(target)<<' '<<target->name<<" is killed\n"; }
        if(alive && health<=0){ alive=false; --Base::aliveNumber; cout<<"Protector "<<name<<" is killed\n"; }
    }
    void Caster::launch_attack(Base* target){
        if(!alive) return;
        int factor = dynamic_cast<Protector*>(target)?2:1;
        int dmg = attack*factor;
        if(target->defensive){ int effective = dmg - target->attack; if(effective<0) effective=0; target->health -= effective; }
        else { target->health -= dmg; }
        if(target->alive && target->health<=0){ target->alive=false; --Base::aliveNumber; cout<<role_name(target)<<' '<<target->name<<" is killed\n"; }
        if(alive && health<=0){ alive=false; --Base::aliveNumber; cout<<"Caster "<<name<<" is killed\n"; }
    }
}
using final::Base; using final::Fighter; using final::Protector; using final::Caster;
static Base* baseptrArray[1005];
int main(){
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, h, att, round=0, cnt=0; string roleInput, nameInput; if(!(cin>>n)) return 0; 
    for(int i=0;i<n;++i){
        cin>>roleInput>>nameInput>>h>>att;
        if(roleInput=="Fighter") baseptrArray[i]=new Fighter(nameInput,h,att);
        if(roleInput=="Protector") baseptrArray[i]=new Protector(nameInput,h,att);
        if(roleInput=="Caster") baseptrArray[i]=new Caster(nameInput,h,att);
    }
    while(Base::getAliveNumber()>1){
        ++round;
        for(int i=0;i<n;++i){ Base*& ptr=baseptrArray[i]; if(ptr->isAlive()){ ++cnt; if(cnt==7){ ptr->defend(true); cnt=0; } else ptr->defend(false);} }
        for(int i=0;i<n;++i){ Base*& ptr=baseptrArray[i]; if(ptr->isAlive() && !ptr->isDefensive()){ for(int j=0;j<n;++j){ if(j!=i && baseptrArray[j]->isAlive()){ ptr->launch_attack(baseptrArray[j]); break; } } } }
    }
    cout<<round<<'\n';
    for(int i=0;i<n;++i) delete baseptrArray[i];
    return 0;
}
