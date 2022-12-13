# Skipod_2_or_Distributed_systemns
This is repo for task in 7 sem course "Distributed systems"



Write this command in the directory 2 to run second task:
docker pull abouteiller/mpi-ft-ulfm
alias make='docker run -v $PWD:/sandbox:Z abouteiller/mpi-ft-ulfm make'
alias mpirun='docker run -v $PWD:/sandbox:Z abouteiller/mpi-ft-ulfm mpirun --oversubscribe -mca btl tcp,self'
make mainmpi
export TMPDIR=/tmp
$HOME/opt/usr/local/bin/mpicc -o task task1.cpp 
$HOME/opt/usr/local/bin/mpirun -np 16 --oversubscribe ./task
