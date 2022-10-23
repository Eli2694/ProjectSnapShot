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
int countNumOfSnapShot(t_SnapShot* headOfSnapshot);

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
		WriteHeaderFile.SnapShotCount = countNumOfSnapShot(SnapShot_Head);
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
	t_Process* Process;
	t_DLL* DLL;
	while (SnapShotHead)
	{
		fwrite(SnapShotHead, sizeof(t_SnapShot), 1, out2);
		Process = SnapShotHead->ListOfProcesses;
		while (Process)
		{
			fwrite(Process, sizeof(t_Process), 1, out2);
			DLL = Process->ListOfDlls;
			while (DLL)
			{
				fwrite(DLL, sizeof(t_DLL), 1, out2);
				DLL = DLL->next;
			}

			Process = Process->next;
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

int countNumOfSnapShot(t_SnapShot* headOfSnapshot)
{
	int CSnapShot = 0;
	t_SnapShot* curr = headOfSnapshot;
	while (curr)
	{
		CSnapShot++;
		curr = curr->next;
	}

	return CSnapShot;
}