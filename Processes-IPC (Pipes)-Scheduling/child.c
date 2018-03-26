#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <time.h>
#define BUFSIZE 100 


int main(int argc, char* argv[])
{
	float nextburstTime;
	float alpha = 0.5;
	float burstTime;
	int count = 0;
	HANDLE hStdin, hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	char message[100];
	int bytesToWrite;
	int bytesWritten = 0;
	bytesToWrite = strlen(message);

	if (argc != 2)
	{
		printf("error in child process...now exiting %d\n", argv[0]);
		system("pause");
		exit(0);
	}

	if ((hStdout == INVALID_HANDLE_VALUE) || (hStdin == INVALID_HANDLE_VALUE))
	{
		printf("Could not do it");
		exit(0);
	}

	while (count<5)
	{

		if (!ReadFile(hStdin, message, BUFSIZE, &bytesToWrite, NULL)) {
			printf("Waiting from pipe");
			exit(0);

		}

		burstTime = atof(message);


		srand(time(NULL));
		float randomNum = rand() % 300 + 50;

		nextburstTime = alpha*randomNum + (1 - alpha) * (burstTime);
		burstTime = nextburstTime;

		char messageTo[100];
		sprintf_s(messageTo, sizeof(messageTo), "%.2f", burstTime);
	
		bytesToWrite = 0;
		bytesWritten = 0;
		bytesToWrite = strlen(messageTo);
		
		if (!WriteFile(hStdout, messageTo, bytesToWrite, &bytesWritten, NULL)) {
			printf("Can not Write To File");
			exit(0);

		}
		count++;
	}










	system("pause");
	CloseHandle(hStdout);
	CloseHandle(hStdin);
	return 1;
}
