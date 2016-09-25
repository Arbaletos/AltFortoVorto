class Charo
{
  private:
    int hp,   	 //Health
        mhp,     //Maximum Health
        eva,     //Evasion Rate
        atk,     //Attack Rate
        dmg,     //Damage
        speed,   //Initiative
        id,      //ID
        tired,   //Acted this turn au ne
	exist;   //cxu this Charo really exists?
    const char *name;  //Name of Charo

  public:
    Charo()
    {
      static int curid = 1;
      
      hp = 0;
      mhp = 0;
      atk = 0;
      eva = 0;
      dmg = 0;
      speed = 0;
      name = NULL;
      id = curid++;
      tired = 0;
      exist = 0;
    }

    Charo recreate(Chars kio, const char* kioname)
    {
      hp = kio.h;
      mhp = kio.h;
      atk = kio.a;
      eva = kio.e;
      dmg = kio.d;
      speed = kio.i;
      name = kioname;
      tired = 0;
      exist = 1;
      if (!kio.h) exist = 0;
    }

    char* list(char *buf)
    {
      sprintf(buf,"%s:\nHP:%d/%d\nPower:%d\nAttack Rate:%d\nDefense Rate:%d\nInitiative:%d\n",name,hp,mhp,dmg,atk,eva,speed);
      return buf;
    }

    char getSymbol()
    {
      if (!this->exist) return ' ';
      if (this->hp<=0) return '+';
      return name[0];
    }

    int getMaxHealth()
    {
      return this->mhp;
    }

    int getDamage()
    {
      return this->dmg;
    }
   
    int getEvasion()
    {
      return this->eva;
    }

    int getAttack()
    {
      return this->atk;
    }


    const char* getName()
    {
      return this->name;
    }

    int getInitiative()
    {
      return this->speed;
    }


    int isExist()
    {
      return this->exist;
    }

    int isAlive()
    {
	return (this->hp>=0);
    }
    
    int isTired()
    {
      return this->tired;
    }

    void setTired(int nTired)
    {
      this->tired = nTired;
    }
    
    void upHealth()
    {
      this->hp++;
      this->mhp++;
    }

    void upEvasion()
    {
      this->eva++;
    }

    void upAttack()
    {
      this->atk++;
    }

    void upDamage()
    {
      this->dmg++;
    }

    void changeHealth(int kiom)
    {
      hp+=kiom;
      if (hp>mhp) hp = mhp;
    }

};

battleRes Charo_attack(battleRes ret,int kiuatk,int kiudmg,int kiuneva)
{	
  int strikes = kiuatk / 100*2;
  if(!strikes) strikes = 1;
  int rate = kiuatk / strikes;
  int eva = kiuneva / 100*2;
  if(!eva) eva = 1;
  int evarate = kiuneva/eva;
  int i;
  int alldamage=0;
  ret.bstrikes = 0;
  ret.strikes = strikes;
  for (i=0; i<strikes; i++)
  {
    if (rate>= rand()%100)
    {
      int curdamage = kiudmg;
      int j;
      int mul = 0;
      for (j=0;j<eva;j++)
      {
        if(evarate>=rand()%100) mul++;
      }
      if (mul) curdamage/=2*mul;
      alldamage+=curdamage;
      ret.bstrikes++;
    }
  }
  ret.dmg=alldamage;
  return ret;
}

