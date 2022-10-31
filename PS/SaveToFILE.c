#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "SnapShot.h"
#pragma warning(disable:4996)

//function - specification
t_SnapShot* runToHeadOfSanpShot(t_SnapShot* Tail);
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
		LogError("SnapShot.bin",strerror(GetLastError()));
		exit(1);
	}
	else
	{
		WriteHeaderFile.version = 1;
		WriteHeaderFile.SnapShotCount = countNumOfSnapShot(SnapShot_Head);
		if (!WriteHeaderFile.SnapShotCount)
		{
			LogError("No SnapShot Found (SaveToFile)");
			fclose(out);
			exit(1);
		}
		checkWriteToFile = fwrite(&WriteHeaderFile, sizeof(t_HeaderFile), 1, out);
		if (!checkWriteToFile)
		{
			LogError("Problem writing a header to a binary file");
			fclose(out);
			exit(1);
		}
		fclose(out);
	}

	FILE* out2 = fopen("SnapShot.bin", "ab");
	if (!out2)
	{
		LogError("Problam with out2 (SaveToFile)",strerror(GetLastError()));
		exit(1);
	}

	t_SnapShot* SnapShotHead = runToHeadOfSanpShot(Tail);
	t_Process* Process;
	t_DLL* DLL;

	while (SnapShotHead)
	{
		checkWriteToFile = fwrite(SnapShotHead, sizeof(t_SnapShot), 1, out2);
		if (!checkWriteToFile)
		{
			LogError("Problem writing a SnapShotHead to a binary file");
			fclose(out2);
			exit(1);
		}

		Process = SnapShotHead->ListOfProcesses;
		while (Process)
		{
			checkWriteToFile = fwrite(Process, sizeof(t_Process), 1, out2);
			if (!checkWriteToFile)
			{
				LogError("Problem writing a Process to a binary file");
				fclose(out2);
				exit(1);
			}

			DLL = Process->ListOfDlls;
			while (DLL)
			{
				checkWriteToFile = fwrite(DLL, sizeof(t_DLL), 1, out2);
				if (!checkWriteToFile)
				{
					LogError("Problem writing a DLL to a binary file");
					fclose(out2);
					exit(1);
				}

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