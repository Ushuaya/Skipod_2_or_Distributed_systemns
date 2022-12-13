# Skipod_2_or_Distributed_systemns
This is repo for task in 7 sem course "Distributed systems"

Write this command in the directory 1 to run first task:
1) export TMPDIR=/tmp
2) mpicc -o task task1.cpp 
3) mpirun -np 16 --oversubscribe ./task

Write this command in the directory 2 to run second task:
1) docker pull abouteiller/mpi-ft-ulfm
2) alias make='docker run -v $PWD:/sandbox:Z abouteiller/mpi-ft-ulfm make'
3) alias mpirun='docker run -v $PWD:/sandbox:Z abouteiller/mpi-ft-ulfm mpirun --oversubscribe -mca btl tcp,self'
4) make mainmpi

export TMPDIR=/tmp
$HOME/opt/usr/local/bin/mpicc -o task task1.cpp 
$HOME/opt/usr/local/bin/mpirun -np 16 --oversubscribe ./task
