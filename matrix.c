#define _GNU_SOURCE
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/sysinfo.h>
#include<sched.h>
#include<unistd.h>
// gobal variables
struct  Mat{
    int start;
    int end;
};
int **matrix1, **matrix2,**resultmatrix;
int row1,col1,row2,col2;
 // functions prototypes
int**  reader(int* , int*, char []);
void print_matrix(int **, int ,int);
int ** allocate(int , int );
void *Multiply(void *);


int  main(int argc , char **argv)
{
    matrix1=reader(&row1, &col1, "input1.txt");
    print_matrix(matrix1, row1,col1);
    matrix2 = reader(&row2,&col2, "input2.txt");
    print_matrix(matrix2, row2, col2);
    if(col2 != row1)
    {
        perror("cannot do multiplication!!!");
        printf("existing the code..!!!");
        exit(1);
    }
    else
    {
        int cpus = get_nprocs(); //  getting no of cpus
        printf(" No of CUPS %d \n",cpus);
        pthread_t *t; // maing a thread pointer
        resultmatrix = allocate(col2, row1); // allocating the resultent matrix
        int noOfthreads = atoi(argv[1]);
        cpu_set_t mask; // making a cpu object
        CPU_ZERO(&mask); // initizing the cpu
        if(noOfthreads % 2 == 0)
        {
            t =  (pthread_t*)malloc(sizeof(pthread_t) *noOfthreads); // making threads
            struct Mat * data = (struct Mat *)malloc(sizeof(struct Mat));
            data->start =0; data->end=0;
             int thread =0;
             int cpu_count =0;
            for(int i=0 ;i<=col2; i++)
            {
                 for(int j =0; j<=row1; j++)
                {
                  
                    data->start = i;
                    data->end = j;
                   void *ptr = (void*)data;
                    sched_getaffinity(0, sizeof(cpu_set_t), &mask);
                   pthread_create(&t[thread],NULL,Multiply, ptr);
                   sleep(1);
                   thread++;
               }
               CPU_SET(cpu_count,&mask); // setting cpu
               cpu_count++;
           }
             for(int i =0; i<noOfthreads; i++)
            {
                 pthread_join(t[i],NULL);
            }
            printf(" \nprint matrix\n");
            print_matrix(resultmatrix,row1,col2);
        }
        else
        {
            printf("enter a even number of threads..!!! \n");
            exit(1);
        }
        
       
    }
}
// reader function
int**  reader( int* rows, int *cols, char file[])
{
    int **M;
    FILE * f = fopen(file, "r");
    if(f== NULL){
        printf("file not found..!!");
        exit(1);
    }
    fscanf(f,"%d", rows);
    fscanf(f,"%d", cols);
    M = allocate(*rows,*cols);
    for (int i=0;i<*rows; i++)
    {
        for(int j=0; j<*cols; j++)
        {
            fscanf(f, "%d",&M[i][j]);
        }
    }
    fclose(f);
    return M;
    
}
// printing the matrix
void print_matrix(int **Matrix, int row ,int col)
{
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            printf("%d ",Matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}
// allocating the matrix accounding to the row and coloums
int ** allocate(int r , int c )
{
      int **M =(int**) malloc(r * sizeof(int *));
    for(int i = 0; i <=r; i++) {
           M[i] = (int*)malloc(c * sizeof(int));
    }
    return M;
}

// multiplying the martix 
void *Multiply(void * ptr)
{
    struct Mat *Matrix = (struct Mat*)ptr;
    int ans =0;
    for(int row =0;row<col2; row++)
    {
        ans += matrix1[Matrix->start][row]* matrix2[row][Matrix->end];
    }

    resultmatrix[Matrix->start][Matrix->end] = ans;
    pthread_exit(NULL);
}