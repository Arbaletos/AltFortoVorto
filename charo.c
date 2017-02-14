
class Charo
{
  private:
    int 	hp,   	 //Health
        	mhp,     //Maximum Health
        	eva,     //Evasion Rate
        	atk,     //Attack Rate
        	dmg,     //Damage
       	 	speed,   //Initiative
        	id,      //ID
	        tired,   //Acted this turn au ne
		exist;   //cxu this Charo really exists?
    char* 	name;  	 //Name of Charo
    Efektaro*   efektaro;

  public:
    Charo()
    {
      static int curid = 1;
      
      hp = 0;
      mhp = 0;
      atk = 0;
      eva = 0;
      dmg = 0;
//      speed = 0;
      name = NULL;
      id = curid++;
      tired = 0;
      exist = 0;
      efektaro = new Efektaro(this);
    }

    Charo(Chars kio, char* kioname)
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
        efektaro = new Efektaro(this);
    }

    Charo recreate(Chars kio, char* kioname)
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
      if (efektaro) free(efektaro);
      efektaro = new Efektaro(this);
      if (!kio.h) exist = 0;
    }
  
//    void addTrigger(trigger* newt)
//    {
//      efektaro.addTriiger(newt);
/*
      trigeto* curt = ftrigger;
      if (ftrigger == NULL) 
      {
        ftrigger = new trigeto;
        ftrigger->curt = newt;
        ftrigger->next = NULL;
      }
      else 
      {
        while(curt->next!=NULL) curt = curt->next;
        curt->next = new trigeto;
        curt->next->next = NULL;
        curt->next->curt = newt;
      }*/
//    }
    void addEfekt(int type, int ttl, void* val )
    {
      efektaro->addEfekt(type,ttl,val);
    }

    void nextTurn()
    {
      efektaro->refresh(1);
    }
        
    void applyTriggers(battleQueue* bq,int type,int time)
    {
      efektaro->applyTriggers(bq, type, time);
/*
//      printf("%s has triggers to check!\n",name);
      if (ftrigger==NULL) return;
//      printf("%s has triggers to check!\n",name);
      trigeto* curt = ftrigger;
      while (curt!=NULL)
      {
        if (type == curt->curt->type) 
        { 
        if (((type==ON_TIME)&&(time==curt->curt->time))||(type!=ON_TIME))
          {
          printf("Launching trigger!\n");
     //     bq->addAgo(ago,19);
          bq->addAgo(curt->curt->ago,time);
         //// bq->addAgo(curt->curt->ago,time);
          }
        }
        curt = curt->next;
      }
*/
    }

    char* list(char *buf)
    {
      sprintf(buf,"%s:\nHP:%d/%d\nPower:%d\nAttack Rate:%d\nDefense Rate:%d\nInitiative:%d\n\n",name,hp,mhp,dmg,atk,eva,speed);
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
    
    int getHealth()
    {
      return this->hp;
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


    char* getName()
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

