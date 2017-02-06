//battleQueue - Ключевой класс реализации раунда боя: Ибо как содержит действия,которые выполняют баталанты во имя богов сражаючись друг с другом, 
//Почему, спросит дружок-пирожок, максимальная инициатива - 20? всё просто как засунуть яйцо в бутылку! в одной битве сржйтс максимум 18 бичевичей. это 18 точек скорости. +1 на события ДО начала мочилова, и ещё +1 на события ПО ОКОнЧАНИИ мочилова. вот так. спасибо! Не будьте бомжами, друзья, пожалуйста!
//также бэкю может быть заюзана как боевой контекст - т.е. 
/*
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
    
  
  public:
*/
    battleQueue::battleQueue(cell* ncell1, cell* ncell2)
    {
      agoj = 0;
      sorted = 0;
      reakoj = 0;
      cell1 = ncell1;
      cell2 = ncell2;
      int i;
      for (i=0;i<ROUNDLEN;i++)
      {
        len[i] = 0;
        turn[i] = 0;
      }
    }

void  battleQueue::run()
{
  int i;
  int j; 
  Reporto * repo = NULL;
  for (i=0;i<ROUNDLEN;i++)
  {
    for (j=0;j<9;j++)
    {
      if (cell1->mistos[j]->isExist()) cell1->mistos[j]->applyTriggers(this,ON_TIME,i);
      if (cell2->mistos[j]->isExist()) cell2->mistos[j]->applyTriggers(this,ON_TIME,i);
    }
    while (turn[i]<len[i])
    {
      repo = agaro[i][turn[i]]->agu(i+1,this);
      repo -> printRep();
      turn[i]++;
    }
  }
//      while (agoj) aguUnuan();
}

void battleQueue::clear()
{
  int i;
  for (i=0;i<ROUNDLEN;i++)
  {
    len[i] = 0;
    turn[i] = 0;
  }
}

void  battleQueue::addAgo(Ago* enago, int tempo)
{
  agaro[tempo][len[tempo]++] = enago;
   //   agaro[agoj] = enago;
   //   [agoj] = tempo;
   //   agoj++;
   //   sorted = 0;
}
//};
  

/*
    void aguUnuan()
    {
      reporto * repo = NULL;
      if (!sorted) sort();
      repo = agaro[--agoj]->agu(&reaktaro);
      repo -> printRep();
    }
*/


/* 
    void sort()
    {
      int i,j;
      int curmax = 0;
      printf("Sorting Started!");
      for (i=0;i<agoj;i++)
      {
        curmax = i;
        for (j = i+1;j<agoj;j++)
        {
          if (temparo[j]>temparo[curmax]) curmax = j;
        }
        if (curmax!=i) 
        {
          int meztempo = temparo[curmax];
          Ago* mezago = agaro[curmax];
          temparo[curmax] = temparo[i];
          agaro[curmax] = agaro[i];
          temparo[i] = meztempo;
          agaro[i] = mezago;
        }
      }
      printf("Sorting Ended!\n");
      sorted = 1;
    }
*/
    
