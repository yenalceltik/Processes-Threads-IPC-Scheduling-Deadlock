#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
int temp=0;
sem_t source1;
sem_t source2;
sem_t source3;

int counterr=0;
int counterr1=0;

typedef struct
{
	char type;
	int threadNo;
}THREAD_PARAMETERS;

void *primeNum(void *param);
void *copy(void *param);
void *negativeNum(void *param);

int numbers1[1000];//Keeps numbers of numbers1.txt
int numbers2[1000];//Keeps numbers of number2.txt
int newNumbers[2000];//Keep numbers after all operations

char *filename1="numbers1.txt";
char *filename2="numbers2.txt";

void reading_file(char*filename,int arr[]);
void create_file(char *filename);

int main(int argc, char* argv[])
{	

	sem_init(&source1,0,1);
	sem_init(&source2,0,1);
	sem_init(&source3,0,1);

	reading_file(filename1,numbers1);
	create_file(filename1);
	reading_file(filename2,numbers2);
	create_file(filename2);
	
	int threadCount = 6;
	pthread_t tid[threadCount];
	pthread_attr_t attr[threadCount];
	
	THREAD_PARAMETERS* lpParameter;
	
	
	lpParameter = malloc(sizeof(THREAD_PARAMETERS)* threadCount);

 	int  i=0;
	
	for(i=0; i<threadCount; i++)
	{
		if(i<4)
		{
			lpParameter[i].type = "A";
			lpParameter[i].threadNo = i + 1;
			pthread_attr_init(&attr[i]);
			pthread_create(&tid[i],&attr[i],copy,&lpParameter[i]);//Threads A do copy-paste operation.
		}
		else if(i==4)
		{
			lpParameter[i].type = "B";
			lpParameter[i].threadNo = i + 1;
			pthread_attr_init(&attr[i]);
			pthread_create(&tid[i],&attr[i],primeNum,&lpParameter[i]);//Thread B finds prime numbers.
			
		}
		else
		{
			lpParameter[i].type = "C";
			lpParameter[i].threadNo = i + 1;
			pthread_attr_init(&attr[i]);	
			pthread_create(&tid[i],&attr[i],negativeNum,&lpParameter[i]);//Thread C finds negative numbers.
			
		}
	
	}

	for(i=0; i<threadCount; i++)
	{
		pthread_join(tid[i],NULL);
	}
	
		int sizeFinal= sizeof(newNumbers) / sizeof(newNumbers[0]);
        
		
		FILE * fp;
   		int l;
   		fp = fopen ("newFile","w");
		
		
   		for(l = 0; l < sizeFinal;l++){
				
                if (newNumbers[l] !=1111 && newNumbers[l] != 0)
        		fprintf (fp,"%d\n",newNumbers[l]);
   		}
   		
		fclose (fp);
	return 1;
}

void reading_file(char*filename,int arr[])
{
	FILE *pToFile = fopen(filename, "r");
	int line = 0;
	char input[1000];
	
	while (fgets(input, 1000, pToFile))
	{	
		arr[line]=atoi(input);
		line++;
		//printf("Line:%d ->  %s", line, input);
		
	}
	printf("\n\nEnd Of Program\n");
	fclose(pToFile);
	
	remove(filename);
	

}

void create_file(char *filename)
{
 FILE *fp = NULL;

        fp = fopen(filename ,"a");

}

void *negativeNum(void *parameters)
{

	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;

	while(1)
	{ 
		
	  sem_wait(&source1);
	  int size= sizeof(numbers1) / sizeof(numbers1[0]);
	  for(int x=0;x<size;x++)
	  {
		if(numbers1[x]<0)
		{
			numbers1[x]=1111;
		}
		else
		{
		//printf("\n Pozitive Numbers: %d",numbers1[x]);

		}

	  }
	  sem_post(&source1);
	  
	  sem_wait(&source2);
	  int size1= sizeof(numbers2) / sizeof(numbers2[0]);
	  for(int r=0;r<size1;r++)
	  {
		if(numbers2[r]<0)
		{
			numbers2[r]=1111;
		}
		else
		{
		//printf("\n Pozitive Numbers: %d",numbers2[r]);

		}

	  }

	  sem_post(&source2);


	  sem_wait(&source3);
	  int size2= sizeof(newNumbers) / sizeof(newNumbers[0]);
	  for(int k=0;k<size2;k++)
	  {
		if(newNumbers[k]<0)
		{
			newNumbers[k]=1111;
		}
		else
		{
		//printf("\n Pozitive Numbers: %d",newNumbers[k]);

		}

	  }

	  sem_post(&source3);
	  if(counterr+counterr1>=2000)
		pthread_exit(0);
	  		
	}
}


void *primeNum(void *parameters)
{
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;

	while(1){
	
	sem_wait(&source1);
//This is for checking prime numbers in file "numbers1.txt".
	int j;
	int i;
	int counter;
	int size= sizeof(numbers1) / sizeof(numbers1[0]);
    for(i=0;i<size;i++)
	{
	int counter=0;
	if(numbers1[i]>0)
	{
		for(j=2;j<numbers1[i];j++)

	  {

	      if(numbers1[i]%j==0)
              {
	           counter=1;
	           break;
	       }
	  }
	  if(counter==0)
	  {
	    numbers1[i]=1111;	
	    //printf("\n It ıs prime: \t %d",numbers1[i]);
	  }
	
	}
	
	else{	
		//printf("\n İt is negative number");
	}
	 
	}
	sem_post(&source1);

	sem_wait(&source2);

//This is for checking prime numbers in file "numbers2.txt"
	int q;
	int p;
	int counter1;
	int size1= sizeof(numbers2) / sizeof(numbers2[0]);
        for(p=0;p<size1;p++)
	{
	int counter1=0;
	if(numbers2[p]>0)
	{
		for(q=2;q<numbers2[p];q++)

	  {

	      if(numbers2[p]%q==0)
              {
	           counter1=1;
	           break;
	       }
	  }
	  if(counter1==0)
	  {
	    numbers2[p]=1111;	
	    //printf("\n It ıs prime: \t %d",numbers2[p]);
	  }
	
	}
	
	else{	
		//printf("\n İt is negative number");
	}
	 
	}


	sem_post(&source2);
	sem_wait(&source3);

//This is for checking prime numbers in the final array.
	int w;
	int z;
	int counter2;
	int size2= sizeof(newNumbers) / sizeof(newNumbers[0]);
        for(z=0;z<size2;z++)
	{
	int counter2=0;
	if(newNumbers[z]>0)
	{
		for(w=2;w<newNumbers[z];w++)

	  {

	      if(newNumbers[z]%w==0)
              {
	           counter2=1;
	           break;
	       }
	  }
	  if(counter2==0)
	  {
	    newNumbers[z]=1111;	
	   // printf("\n It ıs prime: \t %d",newNumbers[z]);
	  }
	
	}
	
	else{	
		//printf("\n İt is negative number");
	}
	 
	}
	sem_post(&source3);
	if(counterr+counterr1>=2000)
		pthread_exit(0);
	
	}
	
}



void *copy(void *parameters)
{	
	
	
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;
	while(1){
	if (counterr+counterr1 >=2000)
		pthread_exit(0);

	sem_wait(&source1);
	sem_wait(&source3);
	int rand2 = rand() % 10;
	int i =0;
	for(i = 0; i < rand2;i++){
        newNumbers[i+temp]=numbers1[counterr+i];
   	}
	
	counterr+=rand2;
	temp+=rand2;
	sem_post(&source3);
	sem_post(&source1);


	sem_wait(&source2);
	sem_wait(&source3);

	int rand1 = rand() % 10;
	for(i = 0; i < rand1;i++){
        newNumbers[i+temp]=numbers2[counterr1+i];
   	}
	
	counterr1+=rand1;
	temp+=rand1;
	
	sem_post(&source3);
	sem_post(&source2);

	

	}
	

	
}
