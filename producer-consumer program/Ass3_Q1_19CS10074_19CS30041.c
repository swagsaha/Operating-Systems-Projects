#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/types.h>
#define shmsize 1000


int r1,r2,c1,c2;

typedef struct _process_data {
double *A;
double *B;
double *C;
int veclen, i, j;
} ProcessData;



void *mult(void *arg){
    ProcessData* p = (ProcessData*)(arg);
    int m;
    /*
    *(*(p->C + p->i) + p->j) = 0;
    for(m=0;m<(p->veclen);m++){
        *(*(p->C + p->i) + p->j) += *(*(p->A + p->i) + m) * *(*(p->B + m) + p->j);
    }
    printf("%f\n",*(*(p->C + p->i) + p->j));*/
    *(p->C)=0.0;
    for(m=0;m<(p->veclen);m++){
        //*(p->C) += (p->A)[p->i][m]*(p->B)[m][p->j];
        *(p->C) += (*((p->A) + ((p->i)*c1 + m) * sizeof(double)))*(*((p->B) + (m*c2 + p->j) * sizeof(double)));
        //printf("\n%f", (*((p->A) + ((p->i)*c1 + m) * sizeof(double))));
        //printf("\n%f", (*((p->B) + (m*c2 + p->j) * sizeof(double))));
    }
    //p->C += sizeof(double);
    //printf("\n%f", *(p->C));
    
    arg = p;
}

int main() {
    int shid1, shid2, shid3, pid, status;
    key_t key = 231;
    double *shdata1, *shdata2, *shdata;
    //double mtx1[10][10], mtx2[10][10];
    int i,j,k;
    struct shmid_ds *shmidds;
    ProcessData a;
    //shid1 = shmget(shmkey,shmsize,shmmode|IPC_CREAT|IPC_EXCL);
    /*shid1 = shmget(key,shmsize,IPC_CREAT | 0666);
    a.C = (double*)shmat(shid1,NULL,0);
    shdata = (double*)shmat(shid1,NULL,0);
    //shdata1 = (double *)shmat(shid1,0,0);
    //shdata = shdata1;
    shdata = a.C;*/
    printf("Enter the rows and columns of matrix 1:");
    scanf("%d%d",&r1,&c1);
    key = 232;
    shid2 = shmget(key,r1*c1*sizeof(double),IPC_CREAT | 0666);
    a.A = (double*)shmat(shid2,NULL,0);
    shdata1 = (double*)shmat(shid2,NULL,0);
    shdata1 = a.A;
    /*printf("\nEnter the rows and columns of matrix 2:");
    scanf("%d%d",&r2,&c2);
    key = 233;
    shid3 = shmget(key,r2*c2*sizeof(double),IPC_CREAT | 0666);
    a.B = (double*)shmat(shid3,NULL,0);
    shdata2 = (double*)shmat(shid3,NULL,0);
    shdata2 = a.B;*/
    //printf("\nShmID: %d shdata: %lf \n",shid1,*shdata1);
    //printf("Enter the rows and columns of matrix 1:");
    //scanf("%d%d",&r1,&c1);
    printf("Enter the matrix 1: \n");
    for(i=0;i<r1;++i)
        for(j=0;j<c1;++j){
            //scanf("%lf",&mtx1[i][j]);
            double f;
            scanf("%lf",&f);
            //*(a.A + (i*r1+j)*sizeof(double)) = f;
            *(a.A) = f;
            a.A += sizeof(double);

        }
    //printf("\nEnter the rows and columns of matrix 2:");
    //scanf("%d%d",&r2,&c2);
    printf("\nEnter the rows and columns of matrix 2:");
    scanf("%d%d",&r2,&c2);
    key = 233;
    shid3 = shmget(key,r2*c2*sizeof(double),IPC_CREAT | 0666);
    a.B = (double*)shmat(shid3,NULL,0);
    shdata2 = (double*)shmat(shid3,NULL,0);
    shdata2 = a.B;
    printf("Enter the matrix 2: \n");   
    for(i=0;i<r2;++i){
        for(j=0;j<c2;++j){
            double f;
            scanf("%lf",&f);
            //*(a.B + (i*r2+j)*sizeof(double)) = f;
            *(a.B) = f;
            a.B += sizeof(double);
        }
    }
    a.B = shdata2;
    a.A = shdata1;   
    //printf("\n Hello,note this...");
    if(r2!=c1) {
        printf("\nCannot Multiply");
        return 0;
    }
    printf("Matrix A:\n");
    for(i=0;i<r1;++i){
        for(j=0;j<c1;++j){
            double f;
            f= *(a.A);
            //f = *(a.A + (i*r1+j)*sizeof(double));
            printf("%f ",f);
            a.A += sizeof(double);
        }
        printf("\n");
    } 
    printf("Matrix B:\n");
    for(i=0;i<r2;++i){
        for(j=0;j<c2;++j){
            double f;
            f= *(a.B);
            //f = *(a.B + (i*r2+j)*sizeof(double));
            printf("%f ",f);
            a.B += sizeof(double);
        }
        printf("\n");
    } 
    key = 231;
    shid1 = shmget(key,r1*c2*sizeof(double),IPC_CREAT | 0666);
    a.C = (double*)shmat(shid1,NULL,0);
    shdata = (double*)shmat(shid1,NULL,0);
    //shdata1 = (double *)shmat(shid1,0,0);
    //shdata = shdata1;
    shdata = a.C;
    a.B = shdata2;
    a.A = shdata1;
    //printf("%d\n",getpid());
    int d = getpid();
    
        for(i=0;i<r1;i++){
            for(j=0;j<c2;j++) {
                
                a.veclen=c1;
                a.i=i;a.j=j;
                
                pid = fork();
                if(pid==0){
                
                    mult(&a);
                    //d=getpid();
                    //printf("%d\n",getpid());
                    
                    exit(0);
                    
                }
                //printf("\nss\n");
                a.C += sizeof(double);
            }
        }
                
    for(i=0;i<r1;i++){
            for(j=0;j<c2;j++) {
                wait(NULL);
            }
    }
    
    a.C = shdata;
    
    printf("\n\n\nResult from Multiplication\n");
    for(i=0;i<r1;++i) {
        printf("\n    ");
        for(j=0;j<c2;j++){
            printf("%f ",*(a.C));
            a.C += sizeof(double);
        }
    }
    
    shmdt((void*)shdata);
    shmdt((void*)a.C);
    shmdt((void*)a.B);
    shmdt((void*)a.A);
    shmctl(shid1,IPC_RMID,shmidds);
    shmctl(shid2,IPC_RMID,shmidds);
    shmctl(shid3,IPC_RMID,shmidds);
    return 0;
}
