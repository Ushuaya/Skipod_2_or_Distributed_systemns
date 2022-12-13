all:
	export TMPDIR=/tmp
	mpicc -o task task1.cpp
	mpirun -np 16 --oversubscribe ./task
clean:
	rm task
