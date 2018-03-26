#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

typedef struct
{	
	int childday;
	int threadNo;
	int total;
}THREAD_PARAMETERS;

DWORD WINAPI threadWork(LPVOID parameters);


int main(int argc, char* argv[])
{
	HANDLE* handles;
	THREAD_PARAMETERS* lpParameter;
	int* threadID;
	int i = 0;
	int threadCount = 4;
	int totalsum = 0;
	char message[100];
	

	if (argc != 2)
	{
		printf("error in child process...now exiting %d\n", argv[0]);
		system("pause");
		exit(0);
	}
	int day;

	for (i = 0; (message[i] = getchar()) != 0; i++)
	{
	}
	day = atoi(message);

	printf("message from parent process: %s, DAY: %d\n", message, day);

	
	handles = malloc(sizeof(HANDLE)* threadCount);
	lpParameter = malloc(sizeof(THREAD_PARAMETERS)* threadCount);
	threadID = malloc(sizeof(int)* threadCount);

	for (i = 0; i < threadCount; i++)
	{
		
		//initialize parameters
		lpParameter[i].threadNo = i + 1;
		lpParameter[i].childday = day;
		lpParameter[i].total = 0;

		handles[i] = CreateThread(NULL, 0, threadWork, &lpParameter[i], 0, &threadID[i]);


		//check errors in creation
		if (handles[i] == INVALID_HANDLE_VALUE)
		{
			printf("error when creating thread\n");
			system("pause");
			exit(0);
		}

		printf("thread %d has started working with id: %d\n", i + 1, threadID[i]);

	}

	WaitForMultipleObjects(threadCount, handles, TRUE, INFINITE);

	
	system("pause");
	return 1;
}

DWORD WINAPI threadWork(LPVOID parameters)
{

	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;



	int milk = 0;
	int biscuit = 0;
	int coke = 0;
	int chips = 0;



	int i = 0;


	FILE* file = fopen("market.txt", "r");
	char line[256];
	char day[256];
	//sprintf_s(day, strlen(day), "%d", param->childday);
	sprintf(day, "%d", param->childday);
	int flag = 1;
	int flag1 = 1;
	int flag2 = 1;

	while (fgets(line, sizeof(line), file) && flag1) {
		char *token = strtok(line, "#(),  ");

		while (token != NULL && flag) {
			//printf("%s\n", token);
			if (strcmp(token, day) == 0) {
				flag = 0;
				break;
			}
			token = strtok(NULL, "#(), ");
		}
		token = strtok(NULL, "#(), ");
		while (token != NULL && flag2) {
				//printf("%s\n", token);
				//token = strtok(NULL, "#(), ");
			if (strcmp(day, token) == 0) {
					flag2 = 0;
					flag1 = 0;
					break;
				}
			
					if (param->threadNo == 1 && strcmp("COKE", token) == 0) {
							coke += 1;
							//printf("coke%d\n", coke);
						

					}
					else if (param->threadNo == 2 && strcmp("CHIPS", token) == 0) {
							chips += 1;
							//printf("chips%d\n", chips);
						
					}
					else if (param->threadNo == 3 && strcmp("BISCUIT", token)==0 ) {
							biscuit += 1;
							//printf("biscuit%d\n", biscuit);
						
					}
					else if (param->threadNo == 4 && strcmp("MILK", token) == 0) {
							milk += 1;
							//printf("milk%d\n", milk);
						
					
				}

				token = strtok(NULL, "#(), ");
			}
		}

		fclose(file);

		
		if (param->threadNo == 1) {

			param->total = coke;
		}

		else	if (param->threadNo == 2) {
			param->total = chips;

		}


		else if (param->threadNo == 3) {

			param->total = biscuit;
		}


		else if (param->threadNo == 4) {

			param->total = milk;
		}
		printf("Thread No 1=Coke -- Thread No 2 =Chips -- Thread No 3 = Biscuit -- Thread No 4=Milk\n");
		printf("Thread No: %d and Number : %d\n", param->threadNo, param->total);

		system("pause");
		return 1;

	
}


