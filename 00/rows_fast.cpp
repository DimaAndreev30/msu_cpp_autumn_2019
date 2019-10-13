#include "timer.h"
#include <cstdlib>
#include <cstdio>

enum {
	DEFAULT_SIZE = 10000
};

//По идее, это быстрая программа - идем вдоль строк
int main (int argc, char* argv [])
{
	//Задаем размер матрицы
	int arrSize = DEFAULT_SIZE;
	if (argc > 1) arrSize = atoi (argv [1]);
	
	//Выделяем память и распределяем указатели по двумерному массиву
	int*  mem  = (int*)  malloc (arrSize*arrSize*sizeof (int));
	int** array = (int**) malloc (arrSize*sizeof (int*));
	
	for (int i = 0, *ptr = mem; i < arrSize; i++)
	{
		for (int j = 0; j < arrSize; j++)
		{
			ptr [j] = i + j;
		}
		
		array [i] = ptr;
		ptr += arrSize;
	}
	
	//Начинаем отсчет:
	{
		Timer t;
	
		volatile int sum = 0;
		for (int i = 0; i < DEFAULT_SIZE; i++)
		{
			for (int j = 0; j < DEFAULT_SIZE; j++)
			{
				sum += array [i][j];
			}
		}
	}
	//Подсчет времени исполнения окончен
	
	delete [] array;
	delete [] mem;
	
	return 0;
}