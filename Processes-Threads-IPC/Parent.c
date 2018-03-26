#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>


#define NO_OF_PROCESS 7

int main(int argc, char* argv[])
{
	STARTUPINFO si[NO_OF_PROCESS];
	PROCESS_INFORMATION pi[NO_OF_PROCESS];
	HANDLE processHandles[NO_OF_PROCESS];
	SECURITY_ATTRIBUTES sa[NO_OF_PROCESS];
	HANDLE writePipe[NO_OF_PROCESS], readPipe[NO_OF_PROCESS];
	char message[20];
	int bytesToWrite = 0;
	int bytesWritten = 0;

	char* lpCommandLine[NO_OF_PROCESS] = { "Child.exe 1", "Child.exe 2", "Child.exe 3", "Child.exe 4", "Child.exe 5", "Child.exe 6", "Child.exe 7" };
	int i = 0;


	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		SecureZeroMemory(&sa[i], sizeof(SECURITY_ATTRIBUTES));
		sa[i].bInheritHandle = TRUE;
		sa[i].lpSecurityDescriptor = NULL;
		sa[i].nLength = sizeof(SECURITY_ATTRIBUTES);

		//creating the pipe
		if (!CreatePipe(&readPipe[i], &writePipe[i], &sa[i], 0)) //use the default buffer size.
		{
			printf("unable to create pipe\n");
			system("pause");
			exit(0);
		}

		//creating variables for child process
		SecureZeroMemory(&si[i], sizeof(STARTUPINFO));
		SecureZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));

		si[i].cb = sizeof(STARTUPINFO);
		si[i].hStdInput = readPipe[i];
		si[i].hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		si[i].hStdError = GetStdHandle(STD_ERROR_HANDLE);
		si[i].dwFlags = STARTF_USESTDHANDLES;

		sprintf_s(message, sizeof(message), "%d", i + 1);
		bytesToWrite = strlen(message);
		bytesToWrite++;

		if (!WriteFile(writePipe[i], message, bytesToWrite, &bytesWritten, NULL))
		{
			printf("unable to write to pipe\n");
			system("pause");
			exit(0);
		}

		printf("number of bytes: %d  --  number of bytes sent: %d\n", bytesToWrite, bytesWritten);



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

	WaitForMultipleObjects(NO_OF_PROCESS, processHandles, TRUE, INFINITE);


	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		free(&si[i]);
		free(&pi[i]);
		free(&sa[i]);
		CloseHandle(readPipe[i]);
		CloseHandle(writePipe[i]);
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
	}

	return 1;
}



