#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#include <time.h>
 
int main(int argc, char* argv[])
{
    int niter = 250000,seed=12345;
    int myid;                       //holds process's rank id
    double x,y;                     //x,y value for the random coordinate
    int i;
    int count=0;                //Count holds all the number of how many good coordinates
    double z;                       //Used to check if x^2+y^2<=1
    double pi;                      //holds approx value of pi
    int reducedcount;                   //total number of "good" points from all nodes
    int reducedniter;                   //total number of ALL points from all nodes
    double startwtime = 0.0, endwtime,total_time; 
    MPI_Init(&argc, &argv);                 //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);           //get rank of node's process


    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",processor_name, world_rank, world_size);

    if(myid == 0)
    {
     startwtime = MPI_Wtime();		
    }	
 

/* Everyone can do the work */

    for (i=0; i < niter; ++i)                  //main loop
    {
        //srand(seed+myid);
        srand48(time(NULL)+myid);               //Give rand() a seed value unique on each node (times are synced)
        x = (double)random()/RAND_MAX;          //gets a random x coordinate
        y = (double)random()/RAND_MAX;          //gets a random y coordinate
        z = sqrt((x*x)+(y*y));              //Checks to see if number in inside unit circle
        
//        printf("Z: %f\n",z);
	if (z<=1)
        {
            ++count;                //if it is, consider it a valid random point
        }
    }
     

    MPI_Reduce(&count,
                   &reducedcount,
                   1,
                   MPI_INT,
                   MPI_SUM,
                   0,
                   MPI_COMM_WORLD);
    MPI_Reduce(&niter,
                   &reducedniter,
                   1,
                   MPI_INT,
                   MPI_SUM,
                   0,
                   MPI_COMM_WORLD);
     
    if (myid == 0)                      //if root process
    {
	endwtime = MPI_Wtime();
        pi = ((double)reducedcount/(double)reducedniter)*4.0;               //p = 4(m/n)
        printf("Pi: %f\n%i\n%d\n", pi, reducedcount, reducedniter);
        //Print the calculated value of pi
 	total_time = (endwtime-startwtime)*1000000;
        printf("Execution time = %f (us) \n", total_time);    
    }
 
    MPI_Finalize();                     //Close the MPI instance
    return 0;
}
