#include <stdio.h>
#include <time.h>
#include <omp.h>

long long num_steps = 100000000;
double step;

int main(int argc, char* argv[])
{
	clock_t start, stop;
	double x, pi, sum = 0.0;
	double tab[50];
	int i,j;
	step = 1. / (double)num_steps;
	omp_set_num_threads(2);
	for (j = 0; j < 30; j++)
	{
		start = clock();
		sum = 0;
#pragma omp parallel
		{int id = omp_get_thread_num();
		tab[j + id] = 0;
#pragma omp for reduction(+:sum)
		for (i = 0; i < num_steps; i++)
		{
			double x = (i + .5) * step;
			tab[j + id] += 4.0 / (1. + x * x);
		}
#pragma omp atomic
		sum += tab[j + id];
		};

		pi = sum * step;
		stop = clock();

		printf("Wartosc liczby PI  %15.12f", pi);
		printf("Czas %f sekund iteracja %d\n", ((double)(stop - start) / 1000.0),j);
	}
	return 0;
}