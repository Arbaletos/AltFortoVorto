//Celaro - Совокупность целей для абилы
class Celaro{
  private:
  Charo* celo;
  public:
  Celaro()
  {
    celo = NULL;
  }

  Celaro(Charo * ncelo)
  {
    celo = ncelo;
  }

  void addCharo(Charo * ncelo)
  {
    celo = ncelo;
  }

  Charo* getCharo()
  {
    return celo;
  }

};
