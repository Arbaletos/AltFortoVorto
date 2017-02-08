#define QUELEN 36
#define REAKLEN 32
#define ROUNDLEN 20

enum efektoj
{
  BLEEDING,
  REGENERATION
};

enum agoj
{
  ATAKI_IUN,
  ATAKI_IEN,
  GET_DAMAGE,
  HEAL_DAMAGE,
  UP_HP,
  DOWN_HP,
  UP_EVA,
  DOWN_EVA,
  UP_DMG, 
  DOWN_DMG,
  UP_ATK,
  DOWN_ATK
} agoj;

enum trigoj
{
  ON_ATTACK,
  ON_HIT,
  ON_GET_DAMAGE, 
  ON_HEAL_DAMAGE, 
  ON_TURNSTART,
  ON_TURNEND,
  ON_TIME
} trigoj;

class Charo;
class Ago;
class battleQueue;
class Efektaro;
class Reporto;
class Celaro;

#include "struktoj.h"


class battleQueue
{
  private:
    Ago* agaro[ROUNDLEN][QUELEN];
//    Reaktaro reaktaro[20];
    int len[ROUNDLEN];
    int turn[ROUNDLEN];
    int agoj;
    int reakoj;
    int sorted;
    cell* cell1;
    cell* cell2;
  
  public:
    battleQueue(cell* cell1, cell* cell2);
    void run();
    void addAgo(Ago* enago, int tempo);
    void clear();
};

class Ago
{
  private:
    int TYPE;
    Charo* kreanto;
    Efektaro* efektaro;
    Celaro* celo;
    int val;

  public:

  Ago(Charo* nkreanto, Efektaro* nefektaro, Celaro* ncelo, int nval, int NTYPE);
  Reporto * agu(int turntime, battleQueue* caller);
};

#include "btl/efektaro.c"
#include "charo.c"
#include "btl/celaro.c"
#include "btl/reporto.c"
#include "btl/ago.c"
#include "btl/queue.c"

