#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
 
int main(int argc, char* argv[]){

  int niter = 1000000;//50000, seed=12345;
  int i;
  int count=0;                    //Numero de coordenadas dentro del circulo
  double x,y;                     // x, y valores para las coordenadas aleatorias
  double z;                       //Usada para verificar si se cumple x^2+y^2<=1
  double pi;                      //Guarda el valor clculado aproximado de pi
 
  //Variables para medicion de tiempo
  struct timeval start, end;
  long mtime, seconds, useconds;
  gettimeofday(&start, NULL);		
  double total_time; 
  //Bucle principal
  for (i=0; i<niter; ++i) {
   
    srand48(time(NULL));         //Give rand() a seed value unique on each node (times are synced)
    //srand(seed); 	
    x = (double)random()/RAND_MAX;    //gets a random x coordinate
    y = (double)random()/RAND_MAX;    //gets a random y coordinate
    z = sqrt((x*x)+(y*y));            //Checks to see if number in inside unit circle
    //printf("z: %f\n",z);
    if (z<=1) {
            ++count; }                //if it is, consider it a valid random point
    }
     
  //Print the calculated value of pi
  pi = ((double)count/(double)niter)*4.0;        //p = 4(m/n)
  printf("Pi: %f\n%i\n%d\n", pi, count, niter);
  
  gettimeofday(&end, NULL);  
  
  seconds = end.tv_sec - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  //mtime = ((seconds)*1000 + useconds/1000.0) + 0.5;
  mtime = useconds;
  printf("El tiempo estimado de calculo es: %ld (us) \n",mtime);
  return 0;
}
