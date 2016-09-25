class charBuf
{
  private:
    char buf[MESSIZE];
    int top;
    int left;
    int wid;
    int hei;
    int visible; 
  public:

    charBuf(int x, int y, int w, int h)
    {
      top = y;
      left = x;
      wid = w;
      hei = h;
      visible = 1;
    }

    void clear()
    {
      int i;
      for (i=0;i<wid*hei;i++)
      {
        buf[i] = ' ';
      }
    }

    void write(char * text, int x, int y)
    {
      int i = 0;
      int nx = x;
      int ny = y;
      for (i=0;i<strlen(text);i++)
      {
        if (nx>=wid)
        {
          nx-=wid;
          ny++;
        }
        if (text[i]=='\n')
        {
          nx = -1;
          ny++;
        }
        else
        {
          putChar(text[i],nx,ny);
        }
        nx++;
      }
    }

    void putChar(char c, int x, int y)
    {
      buf[y*wid+x] = c;
    }

    char getChar(int x, int y)
    {
      if (!visible) return ' ';
      return buf[y*wid+x];
    }

    int getTop()
    {
      return top;
    }

    int getLeft()
    {
      return left;
    }

    int getWidth()
    {
      return wid;
    }

    int getHeight()
    {
      return hei;
    }

    void show()
    {
      visible = 1;
    }

    void hide()
    {
      visible = 0;
    }
};
