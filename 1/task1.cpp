#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define SIZE 4

void send_coords_and_value(int coords[2], int value, int other_coords[2], int other_rank, MPI_Comm comm) {
    MPI_Send(coords, 2, MPI_INT, other_rank, 0, comm);
    MPI_Send(&value, 1, MPI_INT, other_rank, 0, comm);
}
void receive_coords_and_value(int coords[2], int *value, int *other_coords, int other_rank, MPI_Comm comm){
    MPI_Recv(other_coords, 2, MPI_INT, other_rank, 0, comm, MPI_STATUS_IGNORE);
    MPI_Recv(value, 1, MPI_INT, other_rank, 0, comm, MPI_STATUS_IGNORE);
}
//MPI_Send(&a, 1, MPI_INT, other_rank, 0, comm);
//MPI_Recv(&result, 1, MPI_INT, other_rank, 0, comm, &status);
int main(int argc, char **argv)
{
//
//
    MPI_Init(&argc, &argv);
    int rank, tasks;
    MPI_Comm comm;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    int size[2] = {SIZE, SIZE};
    int periodic[2] = {0};
    
    // создание транспьютерной матрицы
    MPI_Cart_create(MPI_COMM_WORLD, 2, size, periodic, 0, &comm);
    int coords[2];
    MPI_Cart_coords(comm, rank, 2, coords);
    time_t t;
    srand((unsigned) time(&t) + rank + 3);
    
    int a = rand() % 1000;
    int result_coords[2] = {INT_MAX, MAX_INT};
    
    // Инициализируем массив в котором хранится текущее минимальное значение текущим значением
    memcpy(result_coords, coords, sizeof coords);

    MPI_Barrier(MPI_COMM_WORLD);
    printf("Coordinates for process %d: (%d, %d)\na[%d][%d] = %d\n", raуnk, coords[0], coords[1], coords[0], coords[1], a);
    MPI_Barrier(MPI_COMM_WORLD);
    
    int result = 0;
    int other_coords[2];
    int other_rank = 0;
    MPI_Status status;
    
    

    // Step 1
    other_coords[1] = coords[1];
    
    switch(coords[0]) {
        case 0:
            other_coords[0] = coords[0] + 1;
            MPI_Cart_rank(comm, other_coords, &other_rank);
            send_coords_and_value(result_coords, a, other_coords, other_rank, comm);
        break;
        case 3:
            other_coords[0] = coords[0] - 1;
            MPI_Cart_rank(comm, other_coords, &other_rank);
            send_coords_and_value(result_coords, a, other_coords, other_rank, comm);
        break;
            
        case 1:
            other_coords[0] = coords[0] - 1;
            MPI_Cart_rank(comm, other_coords, &other_rank);
            receive_coords_and_value(coords, &result, other_coords, other_rank, comm);
            goto Better_res;
        break;
        case 2:
            other_coords[0] = coords[0] + 1;
            MPI_Cart_rank(comm, other_coords, &other_rank);
            receive_coords_and_value(coords, &result, other_coords, other_rank, comm);
            
        Better_res:
            if (result > a) {
                a = result;
                result_coords[0] = other_coords[0];
                result_coords[1] = other_coords[1];
            }
            
        break;
        
        default:
        break;

    }
    MPI_Barrier(comm);
    
    
    // Step 2
    switch(coords[0]) {
        case 1:
            other_coords[0] = coords[0] + 1;
            MPI_Cart_rank(comm, other_coords, &other_rank);
            receive_coords_and_value(coords, &result, other_coords, other_rank, comm);
            
            if (result > a) {
                a = result;
                result_coords[0] = other_coords[0];
                result_coords[1] = other_coords[1];
            }
        break;
        case 2:
            other_coords[0] = coords[0] - 1;
            MPI_Cart_rank(comm, other_coords, &other_rank);
            send_coords_and_value(result_coords, a, other_coords, other_rank, comm);
        break;
        
        default:
        break;

    }
    MPI_Barrier(comm);
    
    
    
    // Step 3
    if (coords[0] == 1 && (coords[1] == 0 || coords[1] == 3)) {
        other_coords[0] = coords[0];
        if (coords[1] == 0) {
            other_coords[1] = coords[1] + 1;
        } else {
            other_coords[1] = coords[1] - 1;
        }
        MPI_Cart_rank(comm, other_coords, &other_rank);
        send_coords_and_value(result_coords, a, other_coords, other_rank, comm);
    }
    
    if (coords[0] == 1 && (coords[1] == 1 || coords[1] == 2)) {
        other_coords[0] = coords[0];
        if (coords[1] == 1) {
            other_coords[1] = coords[1] - 1;
        } else {
            other_coords[1] = coords[1] + 1;
        }
        MPI_Cart_rank(comm, other_coords, &other_rank);
        receive_coords_and_value(coords, &result, other_coords, other_rank, comm);
        
        if (result > a) {
            a = result;
            result_coords[0] = other_coords[0];
            result_coords[1] = other_coords[1];
        }
    }
    MPI_Barrier(comm);
    
    
    
    // Step 4
    if (coords[0] == 1 && coords[1] == 3) {
        other_coords[0] = coords[0];
        other_coords[1] = 1;
        MPI_Cart_rank(comm, other_coords, &other_rank);
        send_coords_and_value(result_coords, a, other_coords, other_rank, comm);
    }
    if (coords[0] == 1 && coords[1] == 1) {
        other_coords[0] = coords[0];
        other_coords[1] = 3;
        MPI_Cart_rank(comm, other_coords, &other_rank);
        receive_coords_and_value(coords, &result, other_coords, other_rank, comm);
        if (result > a) {
            a = result;
            result_coords[0] = other_coords[0];
            result_coords[1] = other_coords[1];
        }
    }
    MPI_Barrier(comm);
    
    // Step 5
    if (coords[0] == 1 && coords[1] == 1) {
        other_coords[0] = 0;
        other_coords[1] = 1;
        MPI_Cart_rank(comm, other_coords, &other_rank);
        send_coords_and_value(result_coords, a, other_coords, other_rank, comm);
    }
    if (coords[0] == 0 && coords[1] == 1) {
        other_coords[0] = 1;
        other_coords[1] = 1;
        MPI_Cart_rank(comm, other_coords, &other_rank);
        receive_coords_and_value(coords, &result, other_coords, other_rank, comm);
        if (result > a) {
            a = result;
            result_coords[0] = other_coords[0];
            result_coords[1] = other_coords[1];
        }
    }
    MPI_Barrier(comm);
    
    
    // Step 6
    if (coords[0] == 0 && coords[1] == 1) {
        other_coords[0] = 0;
        other_coords[1] = 0;
        MPI_Cart_rank(comm, other_coords, &other_rank);
        send_coords_and_value(result_coords, a, other_coords, other_rank, comm);
    }
    if (coords[0] == 0 && coords[1] == 0) {
        other_coords[0] = 0;
        other_coords[1] = 1;
        MPI_Cart_rank(comm, other_coords, &other_rank);
        receive_coords_and_value(coords, &result, other_coords, other_rank, comm);
        if (result > a) {
            a = result;
            result_coords[0] = other_coords[0];
            result_coords[1] = other_coords[1];
        }
    }
    
    MPI_Barrier(comm);
    
    if (coords[0] == 0 && coords[1] == 0) {
        printf("Max result: %d\n", a);
        printf("Max result coords: %d, %d\n", result_coords[0], result_coords[1]);
    }
    MPI_Finalize();
    return 0;
}
