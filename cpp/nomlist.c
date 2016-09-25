class Nomaro
{
  private:
    int len;
    std::vector<char*> list;

  public:
    Nomaro(const char* filename)
    {
      FILE* src = fopen(filename,"r");
      len = 0;
      char curstr[VORTLEN];
      char c;
      while ((c=fgetc(src)) != EOF)
      {
        if (c=='\n') len++;
      }
      list.reserve(len);
      src = freopen(filename,"r",src);
      int i;
      for (i=0;i<len;i++)
      {
	list[i] = new char[VORTLEN];
        fgets(list[i],VORTLEN,src);
	list[i][strlen(list[i])-1] = '\0';
      }
    }

    void print()
    {
      int i;
      for (i=0;i<len;i++)
      {
        printf("%s",list[i]);
      }
    }

    char* getRandom()
    {
      return list[rand()%len];
    }
};
