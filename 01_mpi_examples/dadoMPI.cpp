#include <iostream> 
#include <math.h>
#include "mpi.h"
#include <cstdlib>
//#include <iostream.h>
#define NUM_HISTORIAS 4000//1000
#define NUM_HISTORIAS_div 1000
#define NUM_LADOS 6

using namespace std;

main(int argc, char* argv[])
{  


  int myid, numprocs,comn_sz;
  char processor_name[MPI_MAX_PROCESSOR_NAME];

  int i, seed=12345, lado, bingo;
  float r;
  float pdf[] = {0,0.166667,0.222222,0.1666667,0.1666667,0.111111,0.166667};
  float sum=0, F[NUM_LADOS+1];
  int histogram[NUM_LADOS+1];
  int reducedhistogram[NUM_LADOS+1];
  int histogram_total[] = {0,0,0,0,0,0,0};	


  MPI_Init(&argc, &argv);                 //Start MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);           //get rank of node's process
  MPI_Comm_size(MPI_COMM_WORLD, &comn_sz);
  MPI_Status status;
  // inicializacion de histogram[]
  for (i=0;i<=NUM_LADOS; ++i)
    histogram[i]=0;

  // RAND_MAX es el numero aleatorio mas grande en su sistema
  // Linea para matar la curiosidad 
  //  cout << "RAND_MAX=" << RAND_MAX << endl;

  // Funcion acumulativa F[i]
  for (i=0; i<=NUM_LADOS; ++i){
    sum += pdf[i];
    F[i] = sum;
    // Sin los comentarios, la siguiente linea muestra los valores de F[i]
    //    cout << i << "\t" << pdf[i] << "\t" << F[i] << endl;
  }



  if(myid != 0){
    // inicializacion de la secuencia de numeros aleatorios
 	srand(seed);
	// Monte Carlo lanza NUM_HISTORIAS de veces el dado.
  	// Cada vez el contador del lado resultante 'bingo' es incrementado
  	// en 'histogram[bingo] += 1'
  	for (i=0; i<=NUM_HISTORIAS_div;++i){
    		r=(float)rand()/(float)(RAND_MAX-1);
    		for(lado=1; lado<=NUM_LADOS; ++lado){
      			if (r >= F[lado-1] && r < F[lado]){
	     			bingo = lado;
			        histogram[bingo] += 1;
		     		break;
      			}
    		}	
  	}
	MPI_Send(histogram,NUM_LADOS+1,MPI_INT,0,0,MPI_COMM_WORLD);
  }	
	
  if(myid == 0){
	for(int p=1; p<comn_sz; ++p){
		MPI_Recv(reducedhistogram,NUM_LADOS+1,MPI_INT,p,0,MPI_COMM_WORLD,&status);
		for(int j=1; j<=NUM_LADOS; ++j){
			histogram_total[j] = histogram_total[j] + reducedhistogram[j];
//		    	cout << j << "\t" << reducedhistogram[j] << endl;
		}
	}
    	//cout << "comn_sz" << "\t" << comn_sz << endl;
	for (i=1; i<=NUM_LADOS; ++i)
    	cout << i << "\t" << histogram_total[i] << endl;
 }

  MPI_Finalize();

  return 0;
  
}
