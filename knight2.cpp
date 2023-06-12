#include "knight2.h"

#define FOR(i,l,r) for (int i = l; i < r; i++)

#pragma region Utilities

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

bool isPythagoras(int a, int b, int c) {
    if (a <= 0 || b <= 0 || c <= 0) return false;
    int aS = a*a, bS = b*b, cS = c*c; // a square, b square, c square
    return aS + bS == cS || bS + cS == aS || cS + aS == bS;
}

#pragma endregion

#pragma region Item

BaseItem::BaseItem(ItemType _itemType) : itemType(_itemType), nextItem(nullptr) {}

bool AntiDote::canUse(BaseKnight* knight) {
    if (knight->knightType == KnightType::DRAGON) return false;
    return knight->poisoned;
}

void AntiDote::use(BaseKnight* knight) {
    knight->poisoned = false;
}

bool PhoenixDownI::canUse(BaseKnight* knight) {
    return knight->hp <= 0;
}

void PhoenixDownI::use(BaseKnight* knight) {
    knight->hp = knight->maxhp;
}

bool PhoenixDownII::canUse(BaseKnight* knight) {
    return knight->hp < knight->maxhp / 4;
}

void PhoenixDownII::use(BaseKnight* knight) {
    knight->hp = knight->maxhp;
}

bool PhoenixDownIII::canUse(BaseKnight* knight) {
    return knight->hp < knight->maxhp / 3;
}

void PhoenixDownIII::use(BaseKnight* knight) {
    if (knight->hp <= 0) knight->hp = knight->maxhp / 3;
    else knight->hp += knight->maxhp / 4;
}

bool PhoenixDownIV::canUse(BaseKnight* knight) {
    return knight->hp < knight->maxhp / 2;
}

void PhoenixDownIV::use(BaseKnight* knight) {
    if (knight->hp <= 0) knight->hp = knight->maxhp / 2;
    else knight->hp += knight->maxhp / 5;
}

#pragma endregion

#pragma region Bag

BaseBag::BaseBag(int phoenixdownI, int antidote, int _maxSize) : maxSize(_maxSize), size(0), topItem(nullptr) {
    if (antidote + phoenixdownI <= maxSize) {
        FOR(i,0,phoenixdownI) {
            insertFirst(new(nothrow) PhoenixDownI(ItemType::PhoenixI));
        }

        FOR(i,0,antidote) {
            insertFirst(new(nothrow) AntiDote(ItemType::Antidote));
        }
    } else if (maxSize <= phoenixdownI) {
        FOR(i,0,maxSize) {
            insertFirst(new(nothrow) PhoenixDownI(ItemType::PhoenixI));
        }
    } else {
        FOR(i,0,phoenixdownI) {
            insertFirst(new(nothrow) PhoenixDownI(ItemType::PhoenixI));
        }

        FOR(i,0,maxSize-phoenixdownI) {
            insertFirst(new(nothrow) AntiDote(ItemType::Antidote));
        }
    }

};

DragonBag::DragonBag(int phoenixdownI, int antidote) : BaseBag::BaseBag(phoenixdownI, 0, 14) {}

LancelotBag::LancelotBag(int phoenixdownI, int antidote) : BaseBag::BaseBag(phoenixdownI, antidote, 16) {}

PaladinBag::PaladinBag(int phoenixdownI, int antidote) : BaseBag::BaseBag(phoenixdownI, antidote, 0x7fffffff /* INT_MAX */) {}

NormalBag::NormalBag(int phoenixdownI, int antidote) : BaseBag::BaseBag(phoenixdownI, antidote, 19) {}

BaseBag::~BaseBag() {
    string typeString[17] {};
    typeString[1] = "Antidote";
    typeString[2] = "PhoenixI";
    typeString[4] = "PhoenixII";
    typeString[8] = "PhoenixIII";
    typeString[16] = "PhoenixIV";
    
    BaseItem* currItem = topItem;
    while (currItem != nullptr) {
        topItem = currItem;
        currItem = currItem->nextItem;
        delete topItem;
    } 
}

bool BaseBag::insertFirst(BaseItem* item) {
    // check if the bag is at max capacity
    if (size == maxSize) return false;

    item->nextItem = topItem;
    topItem = item;
    size++;
    return true;
}

// BaseBag is not responsible for deleting returned item
// Users have to delete the item themselves
// returns nullptr if no items found
BaseItem* BaseBag::get(ItemType item) {
    // get first item matches
    BaseItem* currItem = topItem, *prevItem = nullptr;
    while (currItem != nullptr && currItem->itemType != item) {
        prevItem = currItem;
        currItem = currItem->nextItem;
    } 

    // No item found 
    if (currItem == nullptr) return nullptr;
    
    // the first item matches is top item
    if (prevItem == nullptr) {
        topItem = currItem->nextItem;
        currItem->nextItem = nullptr;
        size--;
        return currItem;
    }
    
    // swap topItem into currItem
    // set the next-to-topItem as topItem
    // return currItem
    prevItem->nextItem = topItem;
    topItem = topItem->nextItem;
    prevItem->nextItem->nextItem = currItem->nextItem;
    currItem->nextItem = nullptr;
    size--;
    return currItem;
}

BaseItem* BaseBag::getFirstCanUse(BaseKnight* knight, int item) {
    // get first item matches
    BaseItem* currItem = topItem, *prevItem = nullptr;
    while (currItem != nullptr) {
        if (((currItem->itemType & item) != 0) && currItem->canUse(knight))
            break;
        prevItem = currItem;
        currItem = currItem->nextItem;
    } 

    // No item found 
    if (currItem == nullptr) return nullptr;
    
    // the first item matches is top item
    if (prevItem == nullptr) {
        topItem = currItem->nextItem;
        currItem->nextItem = nullptr;
        size--;
        return currItem;
    }
    
    // swap topItem into currItem
    // set the next-to-topItem as topItem
    // return currItem
    prevItem->nextItem = topItem;
    topItem = topItem->nextItem;
    prevItem->nextItem->nextItem = currItem->nextItem;
    currItem->nextItem = nullptr;
    size--;
    return currItem;
}

BaseItem* BaseBag::pop() {
    if (size == 0) return nullptr;
    BaseItem* tmp = topItem;
    topItem = topItem->nextItem;
    size--;
    return tmp;
}

string BaseBag::toString() const {
    string typeString[17] {};
    typeString[1] = "Antidote";
    typeString[2] = "PhoenixI";
    typeString[4] = "PhoenixII";
    typeString[8] = "PhoenixIII";
    typeString[16] = "PhoenixIV";

    string s("");
    s += "Bag[count=" + to_string(size) + ';';

    BaseItem* currItem = topItem;
    while (currItem != nullptr) {
        s += typeString[currItem->itemType];
        if (currItem->nextItem != nullptr) s += ',';
        currItem = currItem->nextItem;
    } 

    s += ']';
    return s;
}

bool DragonBag::insertFirst(BaseItem* item) {
    if (item->itemType == ItemType::Antidote) return false;
    return BaseBag::insertFirst(item);
}

#pragma endregion

#pragma region Opponent

BaseOpponent::BaseOpponent(int _eventIndex, ArmyKnights* _army) : eventIndex(_eventIndex), army(_army) {}

int MadBear::applyEffect(BaseKnight* knight) {
    if (knight->knightType == LANCELOT || knight->knightType == PALADIN) return gil;

    int levelO = (eventIndex + eventId) % 10 + 1;
    int dmg = baseDamage * max(levelO - knight->level, 0);
    knight->hp -= dmg;

    return gil;
}

int Bandit::applyEffect(BaseKnight* knight) {
    if (knight->knightType == LANCELOT || knight->knightType == PALADIN) return gil;

    int levelO = (eventIndex + eventId) % 10 + 1;

    int dmg = baseDamage * max(levelO - knight->level, 0);
    knight->hp -= dmg;

    return gil;
}

int LordLupin::applyEffect(BaseKnight* knight) {
    if (knight->knightType == LANCELOT || knight->knightType == PALADIN) return gil;

    int levelO = (eventIndex + eventId) % 10 + 1;

    int dmg = baseDamage * max(levelO - knight->level, 0);
    knight->hp -= dmg;

    return gil;
}

int Elf::applyEffect(BaseKnight* knight) {
    if (knight->knightType == LANCELOT || knight->knightType == PALADIN) return gil;

    int levelO = (eventIndex + eventId) % 10 + 1;

    int dmg = baseDamage * max(levelO - knight->level, 0);
    knight->hp -= dmg;

    return gil;
}

int Troll::applyEffect(BaseKnight* knight) {
    if (knight->knightType == LANCELOT || knight->knightType == PALADIN) return gil;

    int levelO = (eventIndex + eventId) % 10 + 1;

    int dmg = baseDamage * max(levelO - knight->level, 0);
    knight->hp -= dmg;

    return gil;
}

int TornBery::applyEffect(BaseKnight* knight) {
    int levelO = (eventIndex + eventId) % 10 + 1;

    if (knight->level >= levelO) {
        knight->level = min(knight->level + 1, 10);
    } else if (knight->knightType != DRAGON) {
        BaseItem* item = knight->bag->get(Antidote);
        if (item == nullptr) {
            FOR(i,0,3) {
                BaseItem* item = knight->bag->pop();
                delete item;
            } 
            knight->hp -= 10;
        } else delete item;
    }

    return 0;
}

int QueenOfCards::applyEffect(BaseKnight* knight) {
    int levelO = (eventIndex + eventId) % 10 + 1;

    if (knight->level >= levelO) return knight->gil;
    if (knight->knightType != PALADIN)
        knight->gil /= 2;
    return 0;
}

int NinaDeRings::applyEffect(BaseKnight* knight) {
    if ((knight->gil >= 50 || knight->knightType == PALADIN) && knight->hp < knight->maxhp / 3) {
        if (knight->knightType != PALADIN)
            knight->gil -= 50;
        knight->hp += knight->maxhp / 5;
    }

    return 0;
}

int DurianGarden::applyEffect(BaseKnight* knight) {
    knight->hp = knight->maxhp;
    return 0;
}

int OmegaWeapon::applyEffect(BaseKnight* knight) {
    if (knight->level == 10 && knight->hp == knight->maxhp || knight->knightType == DRAGON) {
        knight->level = 10;
        knight->gil = 999;
        army->passedOmegaWeapon = true;
    } else knight->hp = 0;

    return 0;
}

int Hades::applyEffect(BaseKnight* knight) {
    if (knight->level == 10 || (knight->level >= 8 && knight->knightType == PALADIN)) {
        army->paladinShield = true;
        army->passedHades = true;
    } else knight->hp = 0;

    return 0;
}

#pragma endregion

#pragma region Knight

string BaseKnight::toString() const {
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    // inefficient version, students can change these code
    //      but the format output must be the same
    string s("");
    s += "[Knight:id:" + to_string(id) 
        + ",hp:" + to_string(hp) 
        + ",maxhp:" + to_string(maxhp)
        + ",level:" + to_string(level)
        + ",gil:" + to_string(gil)
        + ',' + bag->toString()
        + ",knight_type:" + typeString[knightType]
        + "]";

    return s;
}

BaseKnight::BaseKnight(int _id, int _maxhp, int _level, int _gil, int _antidote, int _phoenixdownI, KnightType _knightType) 
    : id(_id), hp(_maxhp), maxhp(_maxhp), level(_level), gil(_gil), knightType(_knightType), poisoned(false)
{
    if (knightType == KnightType::DRAGON) bag = new DragonBag(_phoenixdownI, _antidote);
    else if (knightType == KnightType::LANCELOT) bag = new LancelotBag(_phoenixdownI, _antidote);
    else if (knightType == KnightType::PALADIN) bag = new PaladinBag(_phoenixdownI, _antidote);
    else if (knightType == KnightType::NORMAL) bag = new NormalBag(_phoenixdownI, _antidote);
}

BaseKnight::~BaseKnight() { delete bag; }

BaseKnight* BaseKnight::create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
    if (isPrime(maxhp)) {
        // Paladin
        return new PaladinKnight(id, maxhp, level, gil, antidote, phoenixdownI, KnightType::PALADIN);
    } else if (maxhp == 888) {
        // Lancelot
        return new LancelotKnight(id, maxhp, level, gil, antidote, phoenixdownI, KnightType::LANCELOT);
    } else if (maxhp >= 111) {
        // Dragon
        int a = maxhp % 10, b = (maxhp / 10) % 10, c = maxhp / 100;
        if (isPythagoras(a, b, c)) {
            return new DragonKnight(id, maxhp, level, gil, antidote, phoenixdownI, KnightType::DRAGON);
        }
    }
    
    return new NormalKnight(id, maxhp, level, gil, antidote, phoenixdownI, KnightType::NORMAL);

}

int BaseKnight::fight(BaseOpponent* opponent) {
    int initialHp = this->hp;
    int g = opponent->applyEffect(this);

    // If defeated
    if (hp <= 0) {
        BaseItem* item = bag->getFirstCanUse(this, PhoenixI | PhoenixII | PhoenixIII | PhoenixIV);

        if (item == nullptr) {
            if (gil >= 100) {
                gil -= 100;
                hp = maxhp / 2;
            }

        } else {
            item->use(this);
            delete item;
        }

        g = 0;

    } else {
        // Taking damage meaning that the fight resulted in a tie
        // Meaning that no reward
        if (initialHp != hp) g = 0;
        BaseItem* item = bag->getFirstCanUse(this, PhoenixII | PhoenixIII | PhoenixIV);
        if (item != nullptr) {
            item->use(this);
            delete item;
        }

    }

    return g;
}

#pragma endregion

#pragma region ArmyKnights

ArmyKnights::ArmyKnights(const string& fileName) {
    paladinShield = false;
    guinevereHair = false;
    excaliburSword = false;
    lancelotSpear = false;
    won = false;

    ifstream fhandler(fileName);
    if (fhandler.is_open()) {
        fhandler >> size;
        buffer = new BaseKnight*[size];

        FOR(i,0,size) {
            int MaxHP, level, phoenixdownI, gil, antidote;
            fhandler >> MaxHP >> level >> phoenixdownI >> gil >> antidote;
            buffer[i] = BaseKnight::create(i + 1, MaxHP, level, gil, antidote, phoenixdownI);
        }

    } else throw "Cannot open armyknights file";
}

ArmyKnights::~ArmyKnights() {
    FOR(i,0,size) delete buffer[i];
    delete[] buffer;
}

// non-fainted knight
int ArmyKnights::count() const {
    int c = 0;
    for (int i = 0; i < size; i++) {
        if (buffer[i]->hp > 0) {
            c++;
        }
    }
    return c;
}

BaseKnight* ArmyKnights::lastKnight() const {
    for (int i = size - 1; i >= 0; i--) {
        if (buffer[i]->hp > 0) return buffer[i];
    }
    return nullptr;
}

bool ArmyKnights::hasExcaliburSword() const { return excaliburSword; }
bool ArmyKnights::hasGuinevereHair() const { return guinevereHair; }
bool ArmyKnights::hasLancelotSpear() const { return lancelotSpear; }
bool ArmyKnights::hasPaladinShield() const { return paladinShield; }


void ArmyKnights::printInfo() const {
    cout << "No. knights: " << this->count();
    if (this->count() > 0) {
        BaseKnight * lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
        << ";LancelotSpear:" << this->hasLancelotSpear()
        << ";GuinevereHair:" << this->hasGuinevereHair()
        << ";ExcaliburSword:" << this->hasExcaliburSword()
        << endl
        << string(50, '-') << endl;
}

void ArmyKnights::printResult(bool win) const {
    cout << (win ? "WIN" : "LOSE") << endl;
}

bool ArmyKnights::adventure(Events* events) {

    for (int i = 0; i < events->count(); i++) {
        int ev = events->get(i);

        if (count() <= 0) {
            won = false;
            break;
        }

        if (ev == 0) {
            won = true;
            break;
        }

        if (ev == 1) {
            BaseOpponent* o = new MadBear(i, this);
            fight(o);
        }

        if (ev == 2) {
            BaseOpponent* o = new Bandit(i, this);
            fight(o);
        }

        if (ev == 3) {
            BaseOpponent* o = new LordLupin(i, this);
            fight(o);
        }

        if (ev == 4) {
            BaseOpponent* o = new Elf(i, this);
            fight(o);
        }

        if (ev == 5) {
            BaseOpponent* o = new Troll(i, this);
            fight(o);
        }

        if (ev == 6) {
            BaseOpponent* o = new TornBery(i, this);
            fight(o);
        }

        if (ev == 7) {
            BaseOpponent* o = new QueenOfCards(i, this);
            fight(o);
        }

        if (ev == 8) {
            BaseOpponent* o = new NinaDeRings(i, this);
            fight(o);
        }

        if (ev == 9) {
            BaseOpponent* o = new DurianGarden(i, this);
            fight(o);
        }

        if (ev == 10 && !passedOmegaWeapon) {
            BaseOpponent* o = new OmegaWeapon(i, this);
            fight(o);
        }

        if (ev == 11 && !passedHades) {
            BaseOpponent* o = new Hades(i, this);
            fight(o);
        }

        if (ev == 112)
            pickUpItem(new(nothrow) PhoenixDownII(ItemType::PhoenixII));

        if (ev == 113)
            pickUpItem(new(nothrow) PhoenixDownIII(ItemType::PhoenixIII));

        if (ev == 114)
            pickUpItem(new(nothrow) PhoenixDownIV(ItemType::PhoenixIV));

        if (95 <= ev && ev <= 97) {
            bool* majorItems[] = { &paladinShield, &lancelotSpear, &guinevereHair};
            *(majorItems[ev - 95]) = true;
        }

        if (ev == 98 && paladinShield && lancelotSpear && guinevereHair)
            excaliburSword = true;

        if (ev == 99) {
            if (excaliburSword) {
                won = true;
            } else if (paladinShield && guinevereHair && lancelotSpear) {
                int ultimeciaHP = 5000;
                for (int i = size - 1; i >= 0; i--) {
                    BaseKnight* k = buffer[i];
                    if (k->hp > 0) {
                        if (k->knightType <= 2) {
                            float baseDamage[] { 0.06, 0.05, 0.075};
                            int damage = baseDamage[k->knightType] * (k->level * k->hp * 1.0);
                            ultimeciaHP -= damage;
                            if (ultimeciaHP <= 0) break;
                            else k->hp = 0;
                        } else {
                            k->hp = 0;
                        }
                    }
                }

            } else {
                for (int i = size - 1; i >= 0; i--) {
                    buffer[i]->hp = min(buffer[i]->hp, 0);
                }
            }
        }
        printInfo();
    }

    if (count() > 0) won = true;

    printResult(won);

    return won;
}

bool ArmyKnights::fight(BaseOpponent* opponent) {
    BaseKnight* lk = lastKnight();
    if (lk != nullptr) {
        int gil = lk->fight(opponent);
        if (lk->hp > 0) {
            rewardGil(gil);
            return true;
        }
    }

    return false;
}

void ArmyKnights::rewardGil(int gil) {
    if (gil == 0) return;
    
    int i = size - 1;
    while (i >= 0) {
        if (gil == 0) return;
        if (buffer[i]->hp > 0) {
            if (buffer[i]->gil + gil > 999) {
                gil -= (999 - buffer[i]->gil);
                buffer[i]->gil = 999;
            } else {
                buffer[i]->gil += gil;
                gil = 0;
            }
        }
        i--;
    }
}

// The order of picking up item will be bottom-up
// The last knight will try picking up the item
// If fails then the next-to-last knight will try instead
// The process repeats until the first knight have tried picking up item
bool ArmyKnights::pickUpItem(BaseItem* item) {
    if (item == nullptr) return false;

    for (int i = size - 1; i >= 0; i--) {
        if (buffer[i]->hp > 0 && buffer[i]->bag->insertFirst(item)) {
            return true;
        }
    }

    return false;
}

#pragma endregion

#pragma region Events

Events::Events(const string& fileName) {
    ifstream fHandler(fileName);
    if (fHandler.is_open()) {
        fHandler >> size;

        buffer = new int[size];
        FOR(i,0,size) {
            fHandler >> buffer[i];
        }
    } else throw "Cannot open events file";
}

Events::~Events() { delete[] buffer; }

int Events::count() const { return size; }

int Events::get(int i) const { return buffer[i]; }

#pragma endregion

#pragma region KnightAdventure

KnightAdventure::KnightAdventure() {
    armyKnights = nullptr;
    events = nullptr;
}

KnightAdventure::~KnightAdventure() {
    delete armyKnights;
    delete events;
}

void KnightAdventure::loadArmyKnights(const string& fileName) {
    armyKnights = new ArmyKnights(fileName);
}

void KnightAdventure::loadEvents(const string& fileName) {
    events = new Events(fileName);
}

void KnightAdventure::run() {
    armyKnights->adventure(events);
}

#pragma endregion

