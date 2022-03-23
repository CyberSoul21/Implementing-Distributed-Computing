#Makefile to compile and execute examples in c with mpi
# (c) Wilson Almario <wjalmarior@unal.edu.co>

#GCC = /usr/bin/gcc

#export PATH=/home/gfnun/sharedFolder/mpich-install/bin:$PATH
#export LD_LIBRARY_PATH=/home/gfnun/sharedFolder/mpich-install/lib:$LD_LIBRARY_PATH 


MPIC++ = /home/gfnun/sharedFolder/mpich-install/bin/mpic++
MPICC = /home/gfnun/sharedFolder/mpich-install/bin/mpicc
MPIRUN = /home/gfnun/sharedFolder/mpich-install/bin/mpirun

help:
	@echo "Compile all examples: "
	@echo "make all"
	@echo "        "
	@echo "Compile specific example: "	
	@echo "make compile_namefile "
	@echo "where namefile for example is exp1, cpu, exp2"
	@echo "                           "
	@echo "1. compile_dado"
	@echo "                           "
	@echo "2. compile_dadoMPI"
	@echo "                           "
	@echo "3. compile_cpu"
	@echo "                           "
	@echo "4. compile_exp1"
	@echo "                           "
	@echo "5. compile_exp2"
	@echo "                           "
	@echo "6. dado                    "
	@echo "                           "
	@echo "7. dado_MPI                "
	@echo "                           "
	@echo "8. cpu                     "
	@echo "                           "
	@echo "9. exp1"
	@echo "                           "
	@echo "10. exp2"
	@echo "                           "
	@echo "Delete all generated files"
	@echo "make clean"
all:
	@gcc -o pi_mc_cpu pi_mc_cpu.c -lm
	@$(MPICC) pi_mc_mpi_exp1.c -o pi_mc_mpi_exp1 -lm
	@g++ dado.cpp -o dado
	@echo "Done"
compile_dado: dado.cpp
	@g++ dado.cpp -o dado
	@echo "Done"
dado: dado
	#@./dado
	echo "Not working"
compile_cpu: pi_mc_cpu.c 
	@gcc -o pi_mc_cpu pi_mc_cpu.c -lm
	@echo "pi_mc_cpu compiled successfully"
cpu: pi_mc_cpu
	@./pi_mc_cpu

compile_dadoMPI: dadoMPI.cpp
	$(MPIC++) dadoMPI.cpp -o dadoMPI -lm

dado_MPI: dadoMPI
	#$(MPIRUN) -hosts master,master,master,master,master ./dadoMPI 
	$(MPIRUN) -f hostfile ./dadoMPI 
compile_exp1: pi_mc_mpi_exp1.c
	@$(MPICC) pi_mc_mpi_exp1.c -o pi_mc_mpi_exp1 -lm

exp1: pi_mc_mpi_exp1
	#@$(MPIRUN) -hosts master,master,slave1,slave1 ./pi_mc_mpi_exp1
	@$(MPIRUN) -f hostfile ./pi_mc_mpi_exp1
compile_exp2: pi_mc_mpi_exp2.c
	@$(MPICC) pi_mc_mpi_exp2.c -o pi_mc_mpi_exp2 -lm
exp2: pi_mc_mpi_exp2
#	@$(MPIRUN) -hosts master,master,slave1,slave1,slave1 ./pi_mc_mpi_exp2
	@$(MPIRUN) -f hostfile ./pi_mc_mpi_exp2
clean:
	@rm -rf pi_mc_cpu pi_mc_mpi_exp1 dado pi_mc_mpi_exp2 dadoMPI
	@echo "Done"
