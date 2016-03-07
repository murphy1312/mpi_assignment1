// ConsoleApplication1.cpp


#include "stdafx.h"
/** simple program to test the MPI stuff to see if it works **/
/** includes **/
#include <iostream>
#include <mpi.h>
#include <stdlib.h>     /* srand */
#include <ctime>

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}


int calculateHits(int iterations)
{
	double x = 0;
	double y = 0; 
	double z = 0;
	int i = 0;
	int hits = 0;
	for (i = 0; i<iterations; i++) 
	{
		x = fRand(-1.0, 1.0);
		y = fRand(-1.0, 1.0);
		z = x*x + y*y;
		if (z <= 1)
		{
			hits++;
		}
	}
	return hits;
}

/* master node method */
void coordinator(int iterations, int world_size)
{
	double pi;
	int hits = 0;
	int total_hits;

	total_hits = calculateHits(iterations);

	// gather data from other nodes
	for (int i = 1; i < world_size; i++) 
	{
		MPI_Recv(&hits, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		total_hits += hits;
	}

	// calculate pi
	pi = (double) total_hits / (iterations * world_size)* 4;
	// output result
	std::cout.precision(15);
	std::cout.setf(std::ios::fixed, std::ios::floatfield); // floatfield set to fixed
	std::cout << "calc PI: " << pi << std::endl;
	std::cout << "real PI: 3.14159265359" << std::endl;
	double real_pi = 3.14159265359;
	double sub = real_pi - pi;
	std::cout << "difference: " << sub << std::endl;
	
}

/* slave node method */
void participant(int iterations)
{
	int hits;
	hits = calculateHits(iterations);

	// send data to master node
	MPI_Send(&hits, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

int main(int argc, char** argv) 
{
	MPI_Init(NULL, NULL);
	int iterations = 0;
	int world_size;
	int world_rank;

	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	/* get the number of iterations*/
	sscanf_s(argv[1], "%d", &iterations); 

	/* init seed for rand */

	srand(143452579123 * time(NULL) * world_rank);

	// master node 
	if(world_rank == 0) 
	{
		coordinator(iterations, world_size);
	}
	// other nodes
	else
	{
		participant(iterations);
	}

	MPI_Finalize();
	return 0;
}





