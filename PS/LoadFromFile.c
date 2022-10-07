#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "GetProcessInfo.h"
#include "SnapShot.h"
#include "SaveToFILE.h"
#pragma warning(disable:4996)

t_SnapShot* LoadFromFile()
{
	FILE* in = fopen("SnapShot.bin", "rb");
	if (!in)
	{
		LogError(strerror(GetLastError()));
		return;
	}
	int read;
	t_HeaderFile LoadHeaderFile;
	read = fread(&LoadHeaderFile, sizeof(t_HeaderFile), 1, in);
	if (!read)
	{
		LogError("Problem reading from a binary file");
		fclose(in);
		return NULL;
	}
	t_SnapShot* SnapShot_Information;
	for (int i = 0; i < LoadHeaderFile.SnapShotCount; i++)
	{
		SnapShot_Information = (t_SnapShot*)malloc(sizeof(t_SnapShot));
		fread(&SnapShot_Information->ListOfProcesses, sizeof(t_Process*), 1, in);
		fread(&SnapShot_Information->CountNumberOfSnapShot, sizeof(unsigned int), 1, in);
		fread(&SnapShot_Information->CountNumberOfProcessesInEachSnapShot, sizeof(unsigned int), 1, in);
		fread(&SnapShot_Information->TimeOfSnapShot, sizeof(char[100]), 1, in);
		fread(&SnapShot_Information->next, sizeof(struct s_Process*), 1, in);
		fread(&SnapShot_Information->prev, sizeof(struct s_Process*), 1, in);
		// Number of processes in each SnapShot
		for (int i = 0; i < SnapShot_Information->CountNumberOfProcessesInEachSnapShot; i++)
		{
			
			fread(&SnapShot_Information->ListOfProcesses->ProcessId, sizeof(unsigned int), 1, in);
			fread(&SnapShot_Information->ListOfProcesses->ProcessName, sizeof(char[MAX_PATH]), 1, in);
			fread(&SnapShot_Information->ListOfProcesses->ProcessData, sizeof(PROCESS_MEMORY_COUNTERS), 1, in);
			fread(&SnapShot_Information->ListOfProcesses->NumberOfDLLsInEachProcess, sizeof(unsigned int), 1, in);
			fread(&SnapShot_Information->ListOfProcesses->ListOfDlls, sizeof(t_DLL*), 1, in);
			fread(&SnapShot_Information->ListOfProcesses->next, sizeof(struct s_Process*), 1, in);
			fread(&SnapShot_Information->ListOfProcesses->prev, sizeof(struct s_Process*), 1, in);

			for (int i = 0; i < SnapShot_Information->ListOfProcesses->NumberOfDLLsInEachProcess; i++)
			{
				fread(&SnapShot_Information->ListOfProcesses->ListOfDlls->NameOfDLL, sizeof(char[MAX_PATH]), 1, in);
				fread(&SnapShot_Information->ListOfProcesses->ListOfDlls->next, sizeof(struct s_DLL*), 1, in);
				fread(&SnapShot_Information->ListOfProcesses->ListOfDlls->next, sizeof(struct s_DLL*), 1, in);
				
			}

		}

	}

	fclose(in);

}