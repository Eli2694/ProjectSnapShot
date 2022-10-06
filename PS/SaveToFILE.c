#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "SnapShot.h"
#pragma warning(disable:4996)

//function - specification
t_SnapShot* runToHeadOfSanpShot(t_SnapShot* Tail);
//Variable Declaration
t_HeaderFile HeaderFile;

void WriteToBinaryFile(t_SnapShot* Tail)
{
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
	t_SnapShot* headOfList = runToHeadOfSanpShot(Tail);
	t_SnapShot* beginningOf;
	while (headOfList)
	{
		beginningOf = headOfList;
		checkWriteToFile = fwrite(&headOfList, sizeof(t_SnapShot), 1, out2);
		if (!checkWriteToFile)
		{
			LogError("Problem writing to a binary file");
		}
		while (headOfList->ListOfProcesses)
		{
			checkWriteToFile = fwrite(&headOfList->ListOfProcesses, sizeof(t_Process), 1, out2);
			if (!checkWriteToFile)
			{
				LogError("Problem writing to a binary file");
			}
			while (headOfList->ListOfProcesses->ListOfDlls)
			{
				checkWriteToFile = fwrite(&headOfList->ListOfProcesses->ListOfDlls, sizeof(t_DLL), 1, out2);
				if (!checkWriteToFile)
				{
					LogError("Problem writing to a binary file");
				}
				headOfList->ListOfProcesses->ListOfDlls = headOfList->ListOfProcesses->ListOfDlls->next;
			}
			headOfList->ListOfProcesses = headOfList->ListOfProcesses->next;
		}

		headOfList = headOfList->next;
	}

	fclose(out2);

}

t_SnapShot* runToHeadOfSanpShot(t_SnapShot* Tail)
{
	t_SnapShot* curr = Tail;
	while (curr)
	{
		if (curr->prev == NULL)
		{
			return curr;
		}
		curr = curr->prev;
	}
	
}