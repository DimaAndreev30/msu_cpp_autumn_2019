#include <cstdio>

int main ()
{
	int array [100][100];
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			array [i][j] = 100*i + j;
		}
	}
	
	printf ("%d, %d\n", array [16] [43], *(*array + 16*100 + 43));
}