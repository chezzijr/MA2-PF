#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"
// #define DEBUG

enum ItemType {
    Antidote = 1, 
    PhoenixI = 2, 
    PhoenixII = 4, 
    PhoenixIII = 8, 
    PhoenixIV = 16
};

class BaseItem;
class BaseKnight;
class BaseBag;
class Events;
class ArmyKnights;

class BaseBag {
protected:
    BaseItem* topItem;
    const int maxSize;
    int size;
public:
    BaseBag(int phoenixdownI, int antidote, int maxSize);
    virtual ~BaseBag();
    virtual bool insertFirst(BaseItem * item);
    virtual string toString() const;

    // these returned pointers have to be freed manually
    virtual BaseItem * get(ItemType itemType);
    virtual BaseItem* getFirstCanUse(BaseKnight* knight, int item);
    BaseItem* pop();
};

class LancelotBag : public BaseBag {
public:
    LancelotBag(int phoenixdownI, int antidote);
};
class PaladinBag : public BaseBag {
public:
    PaladinBag(int phoenixdownI, int antidote);
};
class DragonBag : public BaseBag {
public:
    DragonBag(int phoenixdownI, int antidote);
    bool insertFirst(BaseItem* item);
};
class NormalBag : public BaseBag {
public:
    NormalBag(int phoenixdownI, int antidote);
};

class BaseOpponent {
protected:
    int eventIndex;
    int eventId;
    ArmyKnights* army;
public:
    BaseOpponent(int eventIndex, ArmyKnights* army);

    // return >0 as rewarded gil if win
    // else 0
    virtual int applyEffect(BaseKnight* knight) = 0;
};

class MadBear : public BaseOpponent {
    using BaseOpponent::BaseOpponent;
    int eventId = 1;
    int baseDamage = 10;
    int gil = 100;

public:
    int applyEffect(BaseKnight* knight);
};

class Bandit : public BaseOpponent { 
    using BaseOpponent::BaseOpponent;
    int eventId = 2;
    int baseDamage = 15;
    int gil = 150;

public:
    int applyEffect(BaseKnight* knight);
};

class LordLupin : public BaseOpponent { 
    using BaseOpponent::BaseOpponent;
    int eventId = 3;
    int baseDamage = 45;
    int gil = 450;

public:
    int applyEffect(BaseKnight* knight);
};

class Elf : public BaseOpponent { 
    using BaseOpponent::BaseOpponent;
    int eventId = 4;
    int baseDamage = 75;
    int gil = 750;

public:
    int applyEffect(BaseKnight* knight);
};

class Troll : public BaseOpponent { 
    using BaseOpponent::BaseOpponent;
    int eventId = 5;
    int baseDamage = 95;
    int gil = 800;

public:
    int applyEffect(BaseKnight* knight);
};

class TornBery : public BaseOpponent { 
    using BaseOpponent::BaseOpponent;
    int eventId = 6;

public:
    int applyEffect(BaseKnight* knight);
};

class QueenOfCards : public BaseOpponent { 
    using BaseOpponent::BaseOpponent;
    int eventId = 7;

public:
    int applyEffect(BaseKnight* knight);
};

class NinaDeRings : public BaseOpponent { 
    using BaseOpponent::BaseOpponent;
    int eventId = 8;

public:
    int applyEffect(BaseKnight* knight);
};

class DurianGarden : public BaseOpponent { 
    using BaseOpponent::BaseOpponent;
    int eventId = 9;

public:
    int applyEffect(BaseKnight* knight);
};

class OmegaWeapon : public BaseOpponent { 
    using BaseOpponent::BaseOpponent;
    int eventId = 10;

public:
    int applyEffect(BaseKnight* knight);
};

class Hades : public BaseOpponent { 
    using BaseOpponent::BaseOpponent;
    int eventId = 11;

public:
    int applyEffect(BaseKnight* knight);
};

enum KnightType { PALADIN = 0, LANCELOT, DRAGON, NORMAL };

class BaseKnight {
    friend class AntiDote;
    friend class PhoenixDownI;
    friend class PhoenixDownII;
    friend class PhoenixDownIII;
    friend class PhoenixDownIV;

    friend class MadBear;
    friend class Bandit;
    friend class LordLupin;
    friend class Troll;
    friend class Elf;
    friend class TornBery;
    friend class QueenOfCards;
    friend class NinaDeRings;
    friend class DurianGarden;
    friend class OmegaWeapon;
    friend class Hades;

    friend class ArmyKnights;

    int id;
    int hp;
    const int maxhp;
    int level;
    int gil;
    bool poisoned;
    BaseBag * bag;
    KnightType knightType;
public:

    static BaseKnight * create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    BaseKnight(int _id, int _maxhp, int _level, int _gil, int _antidote, int _phoenixdownI, KnightType _knightType);
    virtual ~BaseKnight();
    int fight(BaseOpponent* opponent);
    string toString() const;
};

class NormalKnight : public BaseKnight {
    using BaseKnight::BaseKnight;
};
class LancelotKnight : public BaseKnight {
    using BaseKnight::BaseKnight;
};
class DragonKnight : public BaseKnight {
    using BaseKnight::BaseKnight;
};
class PaladinKnight : public BaseKnight {
    using BaseKnight::BaseKnight;
};

class ArmyKnights {
    friend int Hades::applyEffect(BaseKnight* knight);
    friend int OmegaWeapon::applyEffect(BaseKnight* knight);
    BaseKnight** buffer;
    int size;

    bool paladinShield;
    bool lancelotSpear;
    bool guinevereHair;
    bool excaliburSword;
    bool won;
    bool passedOmegaWeapon;
    bool passedHades;
public:
    ArmyKnights (const string & file_armyknights);
    ~ArmyKnights();
    bool fight(BaseOpponent * opponent);
    bool adventure (Events * events);
    int count() const;
    BaseKnight * lastKnight() const;

    bool hasPaladinShield() const;
    bool hasLancelotSpear() const;
    bool hasGuinevereHair() const;
    bool hasExcaliburSword() const;

    void printInfo() const;
    void printResult(bool win) const;

    bool pickUpItem(BaseItem* item);
    void rewardGil(int gil);
};

class BaseItem {
    friend BaseBag;
    friend DragonBag;
    BaseItem* nextItem;
protected:
    ItemType itemType;
public:
    BaseItem(ItemType _itemType);
    static BaseItem* create(ItemType _itemType, BaseItem* _nextItem);
    virtual bool canUse ( BaseKnight * knight ) = 0;
    virtual void use ( BaseKnight * knight ) = 0;
};

class AntiDote : public BaseItem {
    using BaseItem::BaseItem;
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};
class PhoenixDownI : public BaseItem {
    using BaseItem::BaseItem;
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};
class PhoenixDownII : public BaseItem {
    using BaseItem::BaseItem;
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};
class PhoenixDownIII : public BaseItem {
    using BaseItem::BaseItem;
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};
class PhoenixDownIV  : public BaseItem {
    using BaseItem::BaseItem;
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};


class Events {
    int *buffer;
    int size;
public:
    Events(const string&);
    ~Events();

    int count() const;
    int get(int i) const;
};

class KnightAdventure {
private:
    ArmyKnights * armyKnights;
    Events * events;

public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:

    void loadArmyKnights(const string &);
    void loadEvents(const string &);
    void run();
};

#endif // __KNIGHT2_H__