#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#define NO_OF_PROCESS 5

int main(int argc, char* argv[])
{
	STARTUPINFO si[NO_OF_PROCESS];
	PROCESS_INFORMATION pi[NO_OF_PROCESS];
	HANDLE processHandles[NO_OF_PROCESS];
	SECURITY_ATTRIBUTES sa[NO_OF_PROCESS];
	//HANDLE writePipe[NO_OF_PROCESS], readPipe[NO_OF_PROCESS];

	HANDLE g_hChildStd_IN_Wr[NO_OF_PROCESS];
	HANDLE g_hChildStd_IN_Rd[NO_OF_PROCESS];
	HANDLE g_hChildStd_OUT_Wr[NO_OF_PROCESS];
	HANDLE g_hChildStd_OUT_Rd[NO_OF_PROCESS];
	int count = 0;
	char message[20];
	char messages[20];

	int bytesToWrites = 0;
	bytesToWrites = strlen(messages);
	int bytesWrittens = 0;

	int bytesToWrite = 0;
	int bytesWritten = 0;
	float burstTime[5] = { 300,220,180,45,255 };//Starting burst times.
	int proccessOrder[5] = { 1,2,3,4,5 };
	float tempburstTime;

	char* lpCommandLine[NO_OF_PROCESS] = { "Child.exe 1", "Child.exe 2", "Child.exe 3", "Child.exe 4", "Child.exe 5" };
	int i = 0;

	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		SecureZeroMemory(&sa[i], sizeof(SECURITY_ATTRIBUTES));
		sa[i].bInheritHandle = TRUE;
		sa[i].lpSecurityDescriptor = NULL;
		sa[i].nLength = sizeof(SECURITY_ATTRIBUTES);

		//creating the pipe
		if (!CreatePipe(&g_hChildStd_OUT_Rd[i], &g_hChildStd_OUT_Wr[i], &sa[i], 0)) //use the default buffer size.
		{
			printf("Unable to create pipe\n");
			system("pause");
			exit(0);
		}
		//check errors 
		// Create a pipe for the child process's STDIN. 


		if (!CreatePipe(&g_hChildStd_IN_Rd[i], &g_hChildStd_IN_Wr[i], &sa[i], 0)) //use the default buffer size.
		{
			printf("Unable to create pipe\n");
			system("pause");
			exit(0);
		}

		//creating variables for child process
		SecureZeroMemory(&si[i], sizeof(STARTUPINFO));
		SecureZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));

		si[i].cb = sizeof(STARTUPINFO);
		si[i].hStdInput = g_hChildStd_IN_Rd[i];
		si[i].hStdOutput = g_hChildStd_OUT_Wr[i];
		si[i].hStdError = g_hChildStd_OUT_Wr[i];
		si[i].dwFlags = STARTF_USESTDHANDLES;

		if (!CreateProcess(NULL,
			lpCommandLine[i],
			NULL,
			NULL,
			TRUE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si[i],
			&pi[i]))
		{
			printf("unable to create process: %d\n", i);
			system("pause");
			ExitProcess(0);
		}
		else
		{
			processHandles[i] = pi[i].hProcess;
			printf("parent has created process number %d\n", i);
		}

	}

	while (count<5) {
		int tempIndex;
		for (int index = 0; index<5; index++)
		{
			for (int index2 = 1; index2<5; index2++)
			{
				if (burstTime[index]>burstTime[index2])
				{
					tempburstTime = burstTime[index];
					burstTime[index] = burstTime[index2];
					burstTime[index2] = tempburstTime;

					tempIndex = proccessOrder[index];
					proccessOrder[index] = proccessOrder[index2];
					proccessOrder[index2] = tempIndex;
				}
			}
		}

		for (i = 0; i<5; i++)
		{
			printf("Process Order is P%d \n", proccessOrder[i]);
		}

		for (int a = 0; a < 5; a++) {
			sprintf_s(message, sizeof(message), "%.2f", burstTime[a]);
			bytesToWrite = strlen(message);
			//bytesToWrite++;
			printf("P(%d) started\n", proccessOrder[a]);
			int t = proccessOrder[a]-1;
			if (!WriteFile(g_hChildStd_IN_Wr[t], message, bytesToWrite, &bytesWritten, NULL))
			{
				printf("unable to write to pipe\n");
				system("pause");
				exit(0);
			}

			if (!ReadFile(g_hChildStd_OUT_Rd[t], messages, bytesToWrites, &bytesWrittens, NULL)) {
				printf("Reading");
				exit(0);

			}
			float bt;
			bt = atof(messages);
			printf("P(%d) ended\n", proccessOrder[a]);
			
			burstTime[t] = bt;

		}
		count++;
	}

	WaitForMultipleObjects(NO_OF_PROCESS, processHandles, TRUE, INFINITE);


	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		free(&si[i]);
		free(&pi[i]);
		free(&sa[i]);
		CloseHandle(g_hChildStd_IN_Wr[i]);
		CloseHandle(g_hChildStd_IN_Rd[i]);
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
	}

	return 1;
}