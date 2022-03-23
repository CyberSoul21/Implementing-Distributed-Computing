/*
shell$ g++ -o dado dado.cpp
shell$ ./dado
*/

//#include <iostream.h>
#include<iostream> 
#include <math.h>
#include <unistd.h>
#include <cstdlib>
#define NUM_HISTORIAS 1000
#define NUM_LADOS 6

using namespace std;

int main()
{
  int i, seed=12345, lado, bingo;
  float r;
  //float pdf[] = {0,0.166667,0.222222,0.1666667,0.1666667,0.111111,0.166667};
  float pdf[] = {0,0.166667,0.166667,0.1666667,0.1666667,0.166667,0.166667};
  float sum=0, F[NUM_LADOS+1];
  int histogram[NUM_LADOS+1];

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
    cout << i << "\t" << pdf[i] << "\t" << F[i] << endl;
  }

  // inicializacion de la secuencia de numeros aleatorios
  srand(seed);

  // Monte Carlo lanza NUM_HISTORIAS de veces el dado.
  // Cada vez el contador del lado resultante 'bingo' es incrementado
  // en 'histogram[bingo] += 1'
  for (i=0; i<=NUM_HISTORIAS;++i){
    r=(float)rand()/(float)(RAND_MAX);
  //  cout << "rand()= " << rand() << endl;
    for(lado=1; lado<=NUM_LADOS; ++lado){
      if (r >= F[lado-1] && r < F[lado]){
  bingo = lado;
  histogram[bingo] += 1;
  break;
      }
    }
  }
  // Nos enteramos del resultado del juego...
  for (i=1; i<=NUM_LADOS; ++i)
    cout << i << "\t" << histogram[i] << endl;
  
  return 0;
}
