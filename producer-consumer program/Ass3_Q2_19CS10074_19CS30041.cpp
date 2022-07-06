#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <chrono>

#define max_size 8

using namespace std;

typedef struct {
  int producer_number;
  int status;
  int matrix[1000][1000];
  int id;
}
my_structure;

void delay(float number_of_seconds) {
  clock_t start_time = clock();
  while (clock() < start_time + number_of_seconds * CLOCKS_PER_SEC);
}


typedef struct Queue {
  my_structure arr[max_size]; // array to store queue elements
  int capacity; // maximum capacity of the queue
  int front; // front points to the front element in the queue (if any)
  int rear; // rear points to the last element in the queue
  int count;
  int job_created;
  pthread_mutex_t lock;
  }Queue;
  // current size of the queue






// Utility function to dequeue the front element
void dequeue(Queue* Q) {
  // check for queue underflow
  if (Q->front == -1)
    {
        printf("\nQueue is Empty");
       return;
    }
 
   
    if (Q->front == Q->rear)
    {
        Q->front = -1;
        Q->rear = -1;
    }
    else if (Q->front == Q->capacity-1)
        Q->front = 0;
    else
        Q->front++;
        Q->count--;
 
    
}

// Utility function to add an item to the queue
void insert(Queue* Q,my_structure value) {
  if ((Q->front == 0 && Q->rear == Q->capacity-1) ||
            (Q->rear == (Q->front-1)%(Q->capacity-1)))
    {
        printf("\nQueue is Full");
        return;
    }
 
    else if (Q->front == -1) /* Insert First Element */
    {
        Q->front = Q->rear = 0;
        Q->arr[Q->rear] = value;
    }
 
    else if (Q->rear == Q->capacity-1 && Q->front != 0)
    {
        Q->rear = 0;
        Q->arr[Q->rear] = value;
    }
 
    else
    {
        Q->rear++;
        Q->arr[Q->rear] = value;
    }
    Q->count++;
}

// Utility function to return the front element of the queue
my_structure* peek1(Queue* Q) {
  if (Q->count==0) {
   cout << "Underflow\nProgram Terminated\n";
    exit(EXIT_FAILURE);
  }
  return &(Q->arr[Q->front]);
}
my_structure* peek2(Queue* Q) {
  if (Q->count<=1) {
    cout << "Underflow\nProgram Terminated\n";
    exit(EXIT_FAILURE);
  }
  return &(Q->arr[(Q->front + 1) % Q->capacity]);
}


my_structure* peek3(Queue* Q) {
  if (Q->count<3) {
    cout << "Underflow\nProgram Terminated\n";
    exit(EXIT_FAILURE);
  }
  int ans;
  for(int i=Q->front+2;i<Q->front+Q->count;i++)
  {
  if(Q->arr[i%Q->capacity].status==9)
     ans=i;
  
  
  }
  return &(Q->arr[(ans) % Q->capacity]);
  
}




Queue* create(int shmid,int size)
{ Queue* Q=(Queue*)shmat(shmid,(void*)0,0);
  Q->capacity = size;
  Q->front = -1;
  Q->rear = -1;
  Q->count = 0;
  Q->job_created=0;
  pthread_mutexattr_t lock_attr;
	pthread_mutexattr_init(&lock_attr);
	pthread_mutexattr_setpshared(&lock_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&Q->lock, &lock_attr);
	return Q;

}


void producer(Queue* Q,int M,int index,int pid_t)
{
 while (1) {
      
        if (Q->job_created>=M) 
           break;
          
        
        delay((float) (rand()%4));
       
        while(1)
        {  
        pthread_mutex_lock(&Q->lock);
          
          if (Q->job_created>=M)
         {
           pthread_mutex_unlock(&Q->lock);
           break;
          
        }
        
          
          my_structure ms;
          ms.producer_number = index;
          ms.id = (rand() % 100000) + 1;
          ms.status = 0;
          
          
          for (int i = 0; i < 1000; i++) {
            for (int j = 0; j < 1000; j++) {
              ms.matrix[i][j] = (rand() % 19) - 9;
            }
          }
          
      if(Q->count<Q->capacity)
      {
      
      Q->job_created++;
      insert(Q,ms);
      
      cout<<"\n"<<"Create\t"<<"  producer_number :  "<<ms.producer_number<<"   Process id : \t\t"<<pid_t<<"   matrix status :  "<<ms.status<<"   matrix id :"<<ms.id<<"\n"<<"\n";
      pthread_mutex_unlock(&Q->lock);
      break;
   
      }
      pthread_mutex_unlock(&Q->lock);
    }

        


        
      }
      return;
      

}





void consumer(Queue*Q,int M,int index)
{
   
    
    while (1) 
    {
      
       
       if ((Q->count==1)&&(Q->job_created>=M))
            break;
          
          
          
          delay((float) (rand()%4));
        while(1)
        {
        
          
          pthread_mutex_lock(&Q->lock);
          if((Q->count== 1)&&(Q->job_created>=M))
          {
          pthread_mutex_unlock(&Q->lock);
          break;
          }
          
          if(Q->count==0)
          {
          pthread_mutex_unlock(&Q->lock);
          break;
          
          }
          
          if(Q->count==1)
          {
          pthread_mutex_unlock(&Q->lock);
          break;
          
          }
          
int i,j,k; 
my_structure* m1=peek1(Q);       
my_structure* m2=peek2(Q);
int status=m1->status; 
                    
 if((Q->count>1)&& (status==0)&&(Q->count<Q->capacity))        
{
         cout<<"\n"<<"READING";
         
          my_structure ms;
          ms.producer_number = index;
          ms.id = (rand() % 100000) + 1;

          ms.status = 9;

for(int i=0;i<1000;i++)
{
for(int j=0;j<1000;j++)
ms.matrix[i][j]=0;
}
for (i = 0; i < 500; i++) {
        for (j = 0; j < 500; j++) {
            
            for (k = 0; k < 500; k++)
                ms.matrix[i][j] += m1->matrix[i][k] * m2->matrix[k][j];
        }}


 m1->status=1;
m2->status=1;
insert(Q,ms);

cout<<" Worker Number : "<<index<<" Producer 1 : "<<m1->producer_number<<"   Producer 2 :"<<m2->producer_number<<"  Matrix id 1: "<<m1->id<<"  Matrix id 2 : "<<m2->id<<" block number : "<<status+1<<" Work-done :";
cout<<" COPYING"<<"\n"<<"\n";
pthread_mutex_unlock(&Q->lock);
break;
}





if(Q->count>2&&(status==1))
{
cout<<"READING";

my_structure* ms= peek3(Q);



for (i = 0; i < 500; i++) {
        for (j = 0; j < 500; j++) {
            
            for (k = 0; k < 500; k++)
                (*ms).matrix[i][j] += m1->matrix[i][k+500] * m2->matrix[k+500][j];
        }
        }

m1->status=2;
m2->status=2;
cout<<" Worker Number : "<<index<<" Producer 1 : "<<m1->producer_number<<"   Producer 2 :"<<m2->producer_number<<"  Matrix id 1: "<<m1->id<<"  Matrix id 2 : "<<m2->id<<" block number : "<<status+1<<" Work-done :";
cout<<" ADDING"<<"\n"<<"\n";
pthread_mutex_unlock(&Q->lock);
break;
}

if(Q->count>2&&(status==2))
{
cout<<"READING";
my_structure* ms= peek3(Q);
for (i = 0; i < 500; i++) {
        for (j = 0; j < 500; j++) {
            
            for (k = 0; k < 500; k++)
                (*ms).matrix[i][j+500] += m1->matrix[i][k] * m2->matrix[k][j+500];
        }
        }

m1->status=3;
m2->status=3;
cout<<" Worker Number : "<<index<<" Producer 1 : "<<m1->producer_number<<"   Producer 2 :"<<m2->producer_number<<"  Matrix id 1: "<<m1->id<<"  Matrix id 2 : "<<m2->id<<" block number : "<<status+1<<" Work-done :";
cout<<" ADDING"<<"\n"<<"\n";
pthread_mutex_unlock(&Q->lock);
break;

}

if(Q->count>2&&(status==3))
{
cout<<"READING";
my_structure* ms=peek3(Q);
for (i = 0; i < 500; i++) {
        for (j = 0; j < 500; j++) {
            
            for (k = 0; k < 500; k++)
                (*ms).matrix[i][j+500] += m1->matrix[i][k+500] * m2->matrix[k+500][j+500];
        }
}
m1->status=4;
m2->status=4;
cout<<" Worker Number : "<<index<<" Producer 1 : "<<m1->producer_number<<"   Producer 2 :"<<m2->producer_number<<"  Matrix id 1: "<<m1->id<<"  Matrix id 2 : "<<m2->id<<" block number : "<<status+1<<" Work-done :";
cout<<" ADDING"<<"\n"<<"\n";
pthread_mutex_unlock(&Q->lock);
break;
}


if(Q->count>2&&(status==4))
{
cout<<"READING";
my_structure* ms= peek3(Q);
for (i = 0; i < 500; i++) {
        for (j = 0; j < 500; j++) {
            
            for (k = 0; k < 500; k++)
                (*ms).matrix[i+500][j] += m1->matrix[i+500][k] * m2->matrix[k][j];
        }
}
m1->status=5;
m2->status=5;
cout<<" Worker Number : "<<index<<" Producer 1 : "<<m1->producer_number<<"   Producer 2 :"<<m2->producer_number<<"  Matrix id 1: "<<m1->id<<"  Matrix id 2 : "<<m2->id<<" block number : "<<status+1<<" Work-done :";
cout<<" ADDING"<<"\n"<<"\n";
pthread_mutex_unlock(&Q->lock);
break;

}


if(Q->count>2&&(status==5))
{
cout<<"READING";
my_structure* ms= peek3(Q);
for (i = 0; i < 500; i++) {
        for (j = 0; j < 500; j++) {
            
            for (k = 0; k < 500; k++)
                (*ms).matrix[i+500][j] += m1->matrix[i+500][k+500] * m2->matrix[k+500][j];
        }
}
cout<<" Worker Number : "<<index<<" Producer 1 : "<<m1->producer_number<<"   Producer 2 :"<<m2->producer_number<<"  Matrix id 1: "<<m1->id<<"  Matrix id 2 : "<<m2->id<<" block number : "<<status+1<<" Work-done :";
cout<<" ADDING"<<"\n"<<"\n";
m1->status=6;
m2->status=6;

pthread_mutex_unlock(&Q->lock);
break;
}


if(Q->count>2&&(status==6))
{
cout<<"READING";
my_structure* ms= peek3(Q);
for (i = 0; i < 500; i++) {
        for (j = 0; j < 500; j++) {
            
            for (k = 0; k < 500; k++)
                (*ms).matrix[i+500][j+500] += m1->matrix[i+500][k] * m2->matrix[k][j+500];
        }
}
cout<<" Worker Number : "<<index<<" Producer 1 : "<<m1->producer_number<<"   Producer 2 :"<<m2->producer_number<<"  Matrix id 1: "<<m1->id<<"  Matrix id 2 : "<<m2->id<<" block number : "<<status+1<<" Work-done :";
cout<<" ADDING"<<"\n"<<"\n";
m1->status=7;
m2->status=7;
pthread_mutex_unlock(&Q->lock);
break;


}

if(Q->count>2&&(status==7))
{
cout<<"READING";
my_structure* ms= peek3(Q);
for (i = 0; i < 500; i++) {

        for (j = 0; j < 500; j++) {
            
            for (k = 0; k < 500; k++)
                (*ms).matrix[i+500][j+500] += m1->matrix[i+500][k+500] * m2->matrix[k+500][j+500];
        }



}
cout<<" Worker Number : "<<index<<" Producer 1 : "<<m1->producer_number<<"   Producer 2 :"<<m2->producer_number<<"  Matrix id 1: "<<m1->id<<"  Matrix id 2 : "<<m2->id<<" block number : "<<status+1<<" Work-done :";
cout<<" ADDING"<<"\n"<<"\n";
ms->status=0;
dequeue(Q);
dequeue(Q);
pthread_mutex_unlock(&Q->lock);
break;

}
pthread_mutex_unlock(&Q->lock);
}

}
return;
}
















int main() {
  
  srand(time(0));
  int NP, NW, M;
  cout << "Enter the number of producers" << "\n";
  cin >> NP;
  cout << "\n" << "Enter the number of workers" << "\n";
  cin >> NW;
  cout << "\n" << "Enter the number of matrices" << "\n";
  cin >> M;
 

 
  key_t key;
  int shmid;
  key = ftok("/dev/random", 'b'); // some unique id
  shmid = shmget(key, sizeof(Queue), IPC_CREAT | 0666);
 
 
  Queue*Q=create(shmid,8);
  
  

 


  
  
  auto begin = std::chrono::high_resolution_clock::now();
  


  
  for (int i = 1; i <= NP; i++) {
 
    pid_t pid;
    pid = fork();
    
    
    if(pid<0) cout<<"ERROR PRODUCER CREATION FAILED.";
    if (pid == 0) 
    {
      int pid_t=getpid();
      srand(time(0) ^ i*7);
      producer(Q,M,i,pid_t);
     
      return 0;
         

    }
    
    

  }
  
  


for(int i=1;i<=NW;i++)
{
pid_t pid;
    pid = fork();
    if(pid<0) cout<<"ERROR CONSUMER CREATION FAILED.";
    if (pid == 0) {
     srand(time(0) ^ i);
     
     consumer(Q,M,i);
    
    
     return 0;
     }
}




while(1)
	{   
		// put lock before querying so state change is not possible
		pthread_mutex_lock(&Q->lock);
		if((Q->job_created)==M&&Q->count==1)
		{      
			// calculate end time and time consumed thereafter
			int ans=0;
			my_structure* ms=peek1(Q);
			for(int i=0;i<1000;i++)
			ans+=ms->matrix[i][i];
			cout<<"\n"<<"SUM OF DIAGONAL ELEMENTS : "<<ans<<"\n";
			auto end = std::chrono::high_resolution_clock::now();
			auto consumed = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
			cout<<"We have created and multiplied "<<M<<" matrices, within "<< (float)consumed.count()/1000000 <<" seconds."<<endl;
			pthread_mutex_unlock(&Q->lock);
			break;
		}
		// unlock memory
		pthread_mutex_unlock(&Q->lock);
	}
	
	shmdt(Q);
	shmctl(shmid, IPC_RMID, NULL);

     return 0;




}
