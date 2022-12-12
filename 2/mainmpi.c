//#include <iostream>
//#include <windows.h>
//#include <ctime>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <iomanip>
#include <mpi.h>
#include <mpi-ext.h>
#include <signal.h>

#define KILLED_PROCESS 2


//#define N 10000000
int* ranks_gc;
int nf = 0;
long int N;
int myid;
int rank;
unsigned error_occured = 0;
MPI_Comm main_comm;
int numprocs;


long double rand_point(long double left, long double right);
long double cur_function(long double x);

static void errhandler(MPI_Comm* pcomm, int* perr, ...);
int main(int argc, char *argv[])
{

    int i;
    int rc;
    long double drob,drobSum=0,Result, sum;
    double startwtime = 0.0;
    double endwtime;
    long double leftBorder = -1;
    long double rightBorder = 1;
    
    /*
     
     Получаем количество итераций
     
     */
    N = atoi(argv[1]);
    
    
    /*

    Запускаем рапараллеливание

    */
    if (rc = MPI_Init(&argc, &argv))
    {
      printf("Ошибка запуска, выполнение остановлено \n");
      MPI_Abort(main_comm, rc);
    }
    /*
     
    Определим номер текущего процессора.
     
    */
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    /*
     
    Получение количества процессоров
     
    */
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

    
    
    main_comm = MPI_COMM_WORLD;
    
    if (myid == 0)
    {
        printf("Number of processes: %d \n", numprocs);
    }
        
    /*
     
     Устанавливаем обработчик ошибок
     
     */
    MPI_Errhandler errh;
    MPI_Comm_create_errhandler(errhandler, &errh);
    MPI_Comm_set_errhandler(main_comm, errh);
    MPI_Barrier(main_comm);
    /*
     
    Широковещательная рассылка по всем процессорам
     
    */
    int n;
    MPI_Bcast(&n, 1, MPI_INT, 0, main_comm);
    MPI_Barrier(main_comm);

    /*
     
    Cуммируем всё от каждого процесса в переменной dropsum
     
    */
    for (long long int j = 0; j < N/numprocs; j++)
    {
        drobSum += cur_function(rand_point(leftBorder, rightBorder));
    }
    /*
     
    Исскуственно моделируем отказ
     
    */
    MPI_Barrier(main_comm);
    if (myid == KILLED_PROCESS) {
            raise(SIGKILL);
    }
    MPI_Barrier(main_comm);
    /*
     
    Сделаем редукцию по сумме от суммы получеенной в переменной dropsumm на каждом из процессоров
    в переменную Result
     
    */
    MPI_Reduce(&drobSum, &Result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, main_comm);

    if (myid == 0)
    {
      printf("Answer is: %Lf \n", (rightBorder - leftBorder) * (Result / N));
    }
    /*
     
    Останавливаем выполнение mpi
     
    */
    MPI_Barrier(main_comm);
    MPI_Finalize();
    
    return 0;
}



long double rand_point(long double left, long double right)
{
    return left + (long double)((long double)rand() / ((long double)RAND_MAX) * (right - left));
}

long double cur_function(long double x)
{
    return x*x + x*x*x;
}

static void errhandler(MPI_Comm* pcomm, int* perr, ...) {
    if (myid == 0){
        printf("\nERROR!!\n");
    }
    error_occured = 1;
    int err = *perr;
    char errstr[MPI_MAX_ERROR_STRING];
    int size, nf, len;
    MPI_Group group_f;

    /*
     
     Получаем количесвто отказавших процессов, тип оишбки и меняем константу,
     чтобы формула была верной без учёта результатов отказавшего процессора
     
     */
    MPI_Comm_size(main_comm, &size);
    MPIX_Comm_failure_ack(main_comm);
    MPIX_Comm_failure_get_acked(main_comm, &group_f);
    MPI_Group_size(group_f, &nf);
    MPI_Error_string(err, errstr, &len);
    
    N -= nf*(N/numprocs);
    
    /*
     
     Cоздаем новый коммуникатор без вышедшего из строя процесса
     
     */
    MPIX_Comm_shrink(main_comm, &main_comm);
    MPI_Comm_rank(main_comm, &rank);
    if (myid == 0){
        printf("Rank %d / %d: Notified of error %s. %d found dead\n", rank, size, errstr, nf);
    }
    if (myid == 0){
        printf("There was %d processes\n", numprocs);
    }
    MPI_Comm_size(main_comm,&numprocs);
    if (myid == 0){
        printf("Now %d processes still working \n\n\n", numprocs);
    }
}

