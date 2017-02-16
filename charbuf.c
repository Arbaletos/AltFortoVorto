class charBuf
{
  private:
    char buf[BUFFSIZE];
    int top;
    int left;
    int wid;
    int hei;
    int pos;
    int visible; 
  public:
 
  void clear()
    {
      int i;
      pos =0;
      for (i=0;i<wid*hei;i++)
      {
        buf[i] = ' ';
      }
    }

    charBuf(int x, int y, int w, int h)
    {
      top = y;
      left = x;
      wid = w;
      hei = h;
      pos = 0;
      clear();
      visible = 1;
    }

// 0  1  2  3
// 4  5  6  7
// 8  9 10 11
//12 13 14 15

    void append(char * text, int clr = 0) // Use clear = 1 to flush the buffer before writing.
    {
      int i = 0;
      if (clr) clear();
      for (i=0;i<strlen(text);i++)
      {
        if (i+pos < BUFFSIZE)
        {
          if (text[i]=='\n') pos = (pos + wid) / wid * wid -1;
          else buf[pos] = text[i];
          pos++;
        } 
      }
    }

    void write(char * text, int x, int y)  //Use WRITE only to write text to buffer, und do not merge pseudograph and text info!
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
