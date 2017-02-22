//class Ago
//{
//  private:
//    int TYPE;
//    Charo* kreanto;
//    Efektaro* efektaro;
//    Celaro* celo;
//    int val;

//  public:
  Ago::Ago(Charo* nkreanto, Efektaro* nefektaro, Celaro* ncelo, int nval, int NTYPE)
  {
    TYPE = NTYPE;
    kreanto = nkreanto;
    celo = ncelo;
    efektaro = nefektaro;
    val = nval;
  }

  Reporto * Ago::agu(int turntime, battleQueue* caller)
  {
    if (TYPE == GET_DAMAGE)
    {
        kreanto->applyTriggers(caller,ON_GET_DAMAGE,turntime);
	printf(get_damage_mes_fmt,kreanto->getName(),val);
        kreanto->changeHealth(-val);
        Reporto* ret = new stringRep("Get damage!\n");
        return ret; 
    }
    if (TYPE == HEAL_DAMAGE)
    {
	printf(heal_damage_mes_fmt,kreanto->getName(),val);
        kreanto->applyTriggers(caller,ON_HEAL_DAMAGE,turntime);
        kreanto->changeHealth(val);
        Reporto* ret = new stringRep("Healed damage!\n");
        return ret; 
    }

    if (TYPE == ATAKI_IUN)
    {
      kreanto->applyTriggers(caller,ON_ATTACK,turntime);
      printf ("%s is Attacking %s!\n",kreanto->getName(),celo->getCharo()->getName());
      Charo* targ = celo->getCharo();
      int strikes = kreanto->getAttack() / 100 * 2;
      if (!strikes) strikes = 1;
      int rate = kreanto->getAttack() / strikes;
      int eva = targ->getEvasion() / 100 * 2;
      if (!eva) eva = 1;
      int evarate = targ->getEvasion() / eva;
      int i,j;
      int alldamage = 0;
      
      for(i=0;i<strikes; i++)
      {
         if (rate>=rand() % 100)
         {
           kreanto->applyTriggers(caller,ON_HIT,turntime);
           int curdamage = kreanto->getDamage();
           int mul = 0;
           for (j=0;j<eva;j++)
           {
             if (evarate>=rand()%100) mul++;
           }
           if (mul) curdamage/=2*mul;
           alldamage+=curdamage;
         }
      }
      Reporto * ret = new battleRep(kreanto, targ, strikes, alldamage);
      if (alldamage!=0) caller->addAgo(new Ago(targ,NULL,NULL,alldamage,GET_DAMAGE),turntime);      
      return ret;
    }
  }
//};
