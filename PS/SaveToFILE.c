#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "SnapShot.h"
#pragma warning(disable:4996)

//function - specification
t_SnapShot* runToHeadOfSanpShot(t_SnapShot* Tail);
t_Process* runToHeadOfProcess(t_Process* Process);
t_DLL* runToHeadOfDLL(t_DLL* DLL);

//Variable Declaration
t_HeaderFile WriteHeaderFile;

void WriteToBinaryFile(t_SnapShot* Tail)
{
	if (Tail == NULL)
	{
		LogError("Parameter is empty");
		return;
	}

	int checkWriteToFile;
	FILE* out = fopen("SnapShot.bin", "wb");
	if (!out)
	{
		LogError(strerror(GetLastError()));
		return;
	}
	else
	{
		WriteHeaderFile.version = 1;
		WriteHeaderFile.SnapShotCount = Tail->CountNumberOfSnapShot;
		if (!WriteHeaderFile.SnapShotCount)
		{
			LogError("No SnapShot Found");
			return;
		}
		checkWriteToFile = fwrite(&WriteHeaderFile, sizeof(t_HeaderFile), 1, out);
		if (!checkWriteToFile)
		{
			LogError("Problem writing to a binary file");
		}
		fclose(out);
	}
	FILE* out2 = fopen("SnapShot.bin", "ab");
	if (!out2)
	{
		LogError(strerror(GetLastError()));
		return;
	}
	t_SnapShot* SnapShotHead = runToHeadOfSanpShot(Tail);
	t_SnapShot* beginningOf;
	while (SnapShotHead)
	{
		fwrite(&SnapShotHead->ListOfProcesses, sizeof(t_Process*), 1, out2);
		fwrite(&SnapShotHead->CountNumberOfSnapShot, sizeof(unsigned int), 1, out2);
		fwrite(&SnapShotHead->CountNumberOfProcessesInEachSnapShot, sizeof(unsigned int), 1, out2);
		fwrite(&SnapShotHead->TimeOfSnapShot, sizeof(char[100]), 1, out2);
		fwrite(&SnapShotHead->next, sizeof(struct s_SnapShot*), 1, out2);
		fwrite(&SnapShotHead->prev, sizeof(struct s_SnapShot*), 1, out2);
		while (SnapShotHead->ListOfProcesses)
		{
			fwrite(&SnapShotHead->ListOfProcesses->ProcessId, sizeof(unsigned int), 1, out2);
			fwrite(&SnapShotHead->ListOfProcesses->ProcessName, sizeof(char[MAX_PATH]), 1, out2);
			fwrite(&SnapShotHead->ListOfProcesses->ProcessData, sizeof(PROCESS_MEMORY_COUNTERS), 1, out2);
			fwrite(&SnapShotHead->ListOfProcesses->NumberOfDLLsInEachProcess, sizeof(unsigned int), 1, out2);
			fwrite(&SnapShotHead->ListOfProcesses->ListOfDlls, sizeof(t_DLL*), 1, out2);
			fwrite(&SnapShotHead->ListOfProcesses->next, sizeof(struct s_Process*), 1, out2);
			fwrite(&SnapShotHead->ListOfProcesses->prev, sizeof(struct s_Process*), 1, out2);
			
			while (SnapShotHead->ListOfProcesses->ListOfDlls)
			{
				fwrite(&SnapShotHead->ListOfProcesses->ListOfDlls->NameOfDLL, sizeof(char[MAX_PATH]), 1, out2);
				fwrite(&SnapShotHead->ListOfProcesses->ListOfDlls->next, sizeof(struct s_DLL*), 1, out2);
				fwrite(&SnapShotHead->ListOfProcesses->ListOfDlls->next, sizeof(struct s_DLL*), 1, out2);
				
				if (SnapShotHead->ListOfProcesses->ListOfDlls->next == NULL)
				{
					SnapShotHead->ListOfProcesses->ListOfDlls = runToHeadOfDLL(SnapShotHead->ListOfProcesses->ListOfDlls);
					break;
				}

				SnapShotHead->ListOfProcesses->ListOfDlls = SnapShotHead->ListOfProcesses->ListOfDlls->next;
			}
			if (SnapShotHead->ListOfProcesses->next == NULL)
			{
				SnapShotHead->ListOfProcesses = runToHeadOfProcess(SnapShotHead->ListOfProcesses);
				break;
			}

			SnapShotHead->ListOfProcesses = SnapShotHead->ListOfProcesses->next;
		}

		SnapShotHead = SnapShotHead->next;
	}

	fclose(out2);

}

t_SnapShot* runToHeadOfSanpShot(t_SnapShot* SnapShot)
{
	t_SnapShot* curr = SnapShot;
	while (curr)
	{
		if (curr->prev == NULL)
		{
			return curr;
		}
		curr = curr->prev;
	}
}

t_Process* runToHeadOfProcess(t_Process* Process)
{
	t_Process* curr = Process;
	while (curr)
	{
		if (curr->prev == NULL)
		{
			return curr;
		}
		curr = curr->prev;
	}

}

t_DLL* runToHeadOfDLL(t_DLL* DLL)
{
	t_DLL* curr = DLL;
	while (curr)
	{
		if (curr->prev == NULL)
		{
			return curr;
		}
		curr = curr->prev;
	}
}