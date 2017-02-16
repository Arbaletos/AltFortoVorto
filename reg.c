//#include <string.h>
//#include <stdio.h>

//A O U
//7 8 9 R "Cxu A estas la tutan A columno?"
//4 5 6 S
//1 2 3 T

int parsePos(char *pos)
{
	int i;
	int pre = 0;
	int select = 0;
	int cur = 0;
	int targ = 0;
	for (i=0;i<strlen(pos);i++)
	{
		switch (pos[i])
		{
			case 'A':
				select = 1;
				if (pre=='O')
				{
					select = 3;
				}
				if (pre=='U')
				{
					select = 7;
				}
				pre = 'A';
				break;
			case 'O':
				select = 2;
				if (pre=='A')
				{
					select = 3;
				}
				if (pre=='U')
				{
					select = 6;
				}
				pre = 'O';
				break;
			case 'U':
				select = 4;
				if (pre=='A')
				{
					select = 7;
				}
				if (pre=='O')
				{
					select = 6;
				}
				pre = 'U';
				break;
			case 'R':
				if (pre=='S')
				{
					cur +=select*8;
				}
				if (pre=='T')
				{
					cur+=select*8;
					cur+=select*64;
				}
				cur+=select;
				targ = targ | cur;
				cur = 0;
				pre = 'R';
				break;
			case 'S':
				if (pre=='R')
				{
					cur+=select;
				}
				if (pre=='T')
				{
					cur+=select*64;
				}
				cur+=select*8;
				targ = targ | cur;
				cur = 0;
				pre = 'S';
				break;
			case 'T': 
				if (pre=='R')
				{
					cur+=select;
					cur+=select*8;
				}
				if (pre=='S')
				{
					cur+=select*8;
				}
				cur+=select*64;
				targ = targ | cur;
				cur = 0;
				pre = 'T';
				break;
		}
	}
	return targ;
}

//int main(int argc, char** argv)
//{
//	parsePos("AORT");
//}
