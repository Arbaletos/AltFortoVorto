
#define MESSIZE 800*600/16/16

struct charBuf
{
	char buf[MESSIZE];
	int top;
	int left;
	int wid;
	int hei;
};

struct Charo
{
	int maxhealth;
	int health;
	int atk;
	int eva;
	int damage;
	int speed;
	int id;
	char* name;
	int cr;
	int cd;

	int tired; //Have this Charo acted in this turn?
};


