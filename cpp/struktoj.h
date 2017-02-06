/*
struct reakto
{
  reakt_type type;
  int value;
  void* target;
};
*/
typedef struct
{
  int value;
} u_val;

typedef struct 
{
  int x;
  int y;
  Charo* mistos[9];
} cell;

typedef struct
{
  int w;
  int h;
  Charo* charo;
  cell* cp;
  int ter;
} celo;

typedef struct
{
  int type;
  int time;
  Ago* ago;
} trigger;

struct battleRes
{
	int strikes;
	int bstrikes;
	int dmg;
};

struct Chars
{
	int h;
	int e;
	int a;
	int d;
	int i;
};

