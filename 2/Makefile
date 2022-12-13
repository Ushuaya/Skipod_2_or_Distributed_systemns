task1: task1.out
	mpirun -n 16 --oversubscribe ./task1.out
task1.out: task1.c
	mpicc task1.c -o task1.out

task2: task2.out
	mpirun -n 8 --oversubscribe ./task2.out 64
task2.out: task2.c
	mpicc task2.c -o task2.out

mainmpi: mainmpi.out
	mpirun -n 8 --oversubscribe --with-ft ulfm ./mainmpi.out 100000
mainmpi.out: mainmpi.c
	mpicc mainmpi.c -O0 -o  mainmpi.out


clean:
	rm -rf *.out
