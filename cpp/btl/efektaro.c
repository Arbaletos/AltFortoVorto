//Types:
//Regeneration: Heals amount of hp in end of turn
//Bleeding: Loses amount of hp in end of turn 

//Type-Type:


typedef struct {
  int type;
  int type_type;
  int ttl;
  void* val;  //Учимся делать массивы с дерьмом и есть их.
} efekto;

typedef struct efekteto
{
  efekto* cur;
  efekteto* next;
  efekteto* prev;
} efekteto;

typedef struct trigeto
{
  trigger* curt;
  trigeto* next;
  trigeto* prev;
  efekto*   kreanto;
}trigeto;

class Efektaro
{
  private:
    efekteto* 	efekt1;
    trigeto* 	ftrigger;
    Charo* 	master; //Начальник, которого можно дёргать.
 
  public:
    Efektaro(Charo* nmaster)
    {
      master   = nmaster;
      efekt1   = NULL;
      ftrigger = NULL;
    }
    void refresh(int turn);

    void addEfekt(int type, int ttl, void* val )
    {
      efekteto* cur = efekt1;
      if (efekt1 == NULL)
      {
        efekt1 = new efekteto;
        efekt1->cur = new efekto;
        efekt1->next = NULL;
        efekt1->prev = NULL;
        cur = efekt1;
      }
      else
      {
        while (cur->next!=NULL) cur = cur->next;
        cur->next = new efekteto;
        cur->next->cur = new efekto;
        cur->next->prev = cur;
        cur->next->next = NULL;
        cur = cur->next;
      }
      //Здесь будет огромная страшная функция, да. адресная арифметика это вам не в магазин пойти за едой и умереть.
      if (type==BLEEDING || type==REGENERATION)
      {
       printf("cur:%d\n",cur==NULL);
       cur->cur = new efekto;
       cur->cur->type = type;
       cur->cur->ttl = ttl;
       cur->cur->val = val;
      }
      refresh(0);
      //Выделяет место в нашем упругом графике и добавляет эффект. по аргументам необоходим тип и ттл КАК МИНИМУМ
    }

    
    void addTrigger(trigger* newt, efekto* nkreanto)
    {
      trigeto* curt = ftrigger;
      if (ftrigger == NULL) 
      {
        ftrigger = new trigeto;
        ftrigger->curt = newt;
        ftrigger->prev = NULL;
        ftrigger->next = NULL;
        ftrigger->kreanto = nkreanto;
      }
      else 
      {
        while(curt->next!=NULL) curt = curt->next;
        curt->next = new trigeto;
        curt->next->next = NULL;
        curt->next->prev = curt;
        curt->next->curt = newt;
        curt->next->kreanto = nkreanto;
      }
    }

    void removeTrigger(efekto* key)
    {
      trigeto* curt = ftrigger;
      while (curt!=NULL)
      {
        if (curt->kreanto==key)
        {
          if (curt->next)
          {
            curt->next->prev = curt->prev;
          }
          if (curt->prev)
          {
            curt->prev->next = curt->next;
          }
          trigeto* next = curt->next;
          free(curt);
          curt = next;
        }
        else curt = curt->next;
      }
    }
    
//    void clearTriggers()
//    { 
//      ftrigger = NULL;           //MEMORY LEAK!
//    }

    void applyTriggers(battleQueue* bq,int type,int time)
    {
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
    }

    
};
void Efektaro::refresh(int turn) 
{
  efekteto* cur = efekt1;
  if (cur==NULL) return;
  while (cur!=NULL)
  {
    if (cur->cur->type==BLEEDING)
    {
      trigger* t_bleed = new trigger;
      t_bleed->type = ON_TIME;
      t_bleed->time = 19;
      t_bleed->ago = new Ago(master, NULL, NULL, 1, GET_DAMAGE);
      addTrigger(t_bleed,cur->cur); 
    }
    if (turn)
    {
        //Уменьшает ттл всех эффектов, вычищает тригерню мастера, и забивает её заново.
     }
     cur = cur->next;
  }
}

