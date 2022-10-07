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
t_HeaderFile HeaderFile;

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
		HeaderFile.version = 1;
		HeaderFile.SnapShotCount = Tail->CountNumberOfSnapShot;
		if (!HeaderFile.SnapShotCount)
		{
			LogError("No SnapShot Found");
			return;
		}
		checkWriteToFile = fwrite(&HeaderFile, sizeof(t_HeaderFile), 1, out);
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
		beginningOf = SnapShotHead;
		checkWriteToFile = fwrite(&SnapShotHead, sizeof(t_SnapShot), 1, out2);
		if (!checkWriteToFile)
		{
			LogError("Problem writing to a binary file");
		}
		while (SnapShotHead->ListOfProcesses)
		{
			checkWriteToFile = fwrite(&SnapShotHead->ListOfProcesses, sizeof(t_Process), 1, out2);
			if (!checkWriteToFile)
			{
				LogError("Problem writing to a binary file");
			}
			while (SnapShotHead->ListOfProcesses->ListOfDlls)
			{
				checkWriteToFile = fwrite(&SnapShotHead->ListOfProcesses->ListOfDlls, sizeof(t_DLL), 1, out2);
				if (!checkWriteToFile)
				{
					LogError("Problem writing to a binary file");
				}
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