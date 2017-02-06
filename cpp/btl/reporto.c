class Reporto
{
  protected:
    char bufalo[1024]; 
  public:
    char* printRep()
    {
      printf("%s",bufalo);
      return bufalo;
    }
};

class stringRep : public Reporto
{
  public:
    stringRep(const char* mes)
    {
      sprintf(bufalo,"%s",mes);
    }
};

class battleRep : public Reporto
{
  private:
    Charo* anto;
    Charo* ato;
    int strikes;
    int damage;

  public:
    battleRep(Charo* nanto, Charo* nato, int nstrikes, int ndamage)
    {
      anto = nanto;
      ato = nato;
      strikes = nstrikes;
      damage = ndamage;
      sprintf(bufalo,"%s attacked %s and dealed %d damage by %d hits\n",anto->getName(),ato->getName(),damage,strikes);
    }

};
