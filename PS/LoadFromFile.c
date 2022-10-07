#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "GetProcessInfo.h"
#include "SnapShot.h"
#include "SaveToFILE.h"
#pragma warning(disable:4996)

//function specification
t_SnapShot* SnapShotInfoFromFile(t_SnapShot* oldSample);
void CreateListOfSnapshots(t_SnapShot* newSample);
t_Process* ProcessInfoFromFile(t_Process* oldProcess);
void CreateListOfProcessesFile(t_Process* Process_node);
t_DLL* DLLInfoFromFile(t_DLL* oldDLL);

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
	t_Process* Process_Information;
	t_DLL* DLL_Information;
	for (int i = 0; i < LoadHeaderFile.SnapShotCount; i++)
	{
		DLL_Head = NULL;
		DLL_Tail = NULL;
		Process_Head = NULL;
		Process_Tail = NULL;

		SnapShot_Information = (t_SnapShot*)malloc(sizeof(t_SnapShot));
		fread(&SnapShot_Information->CountNumberOfSnapShot, sizeof(unsigned int), 1, in);
		fread(&SnapShot_Information->CountNumberOfProcessesInEachSnapShot, sizeof(unsigned int), 1, in);
		fread(&SnapShot_Information->TimeOfSnapShot, sizeof(char[100]), 1, in);
		// Number of processes in each SnapShot
		for (int i = 0; i < SnapShot_Information->CountNumberOfProcessesInEachSnapShot; i++)
		{
			Process_Information = (t_Process*)malloc(sizeof(t_Process));
			fread(&Process_Information->ProcessId, sizeof(unsigned int), 1, in);
			fread(&Process_Information->ProcessName, sizeof(char[MAX_PATH]), 1, in);
			fread(&Process_Information->ProcessData, sizeof(PROCESS_MEMORY_COUNTERS), 1, in);
			fread(&Process_Information->NumberOfDLLsInEachProcess, sizeof(unsigned int), 1, in);
			

			for (int i = 0; i < Process_Information->NumberOfDLLsInEachProcess; i++)
			{
				DLL_Information = (t_DLL*)malloc(sizeof(t_DLL));
				fread(&DLL_Information->NameOfDLL, sizeof(char[MAX_PATH]), 1, in);
				CreateListOfDlls(DLLInfoFromFile(DLL_Information));
			}

			CreateListOfProcessesFile(ProcessInfoFromFile(Process_Information));
		}
		CreateListOfSnapshots(SnapShotInfoFromFile(SnapShot_Information));
	}
	fclose(in);

}

t_DLL* DLLInfoFromFile(t_DLL* oldDLL)
{
	t_DLL* newDLL = (t_DLL*)malloc(sizeof(t_DLL));
	strcpy(newDLL->NameOfDLL, oldDLL->NameOfDLL);
	free(oldDLL);
	return newDLL;
}

t_Process* ProcessInfoFromFile(t_Process* oldProcess)
{
	t_Process* newProcess = (t_Process*)malloc(sizeof(t_Process));
	newProcess->NumberOfDLLsInEachProcess = oldProcess->NumberOfDLLsInEachProcess;
	newProcess->ProcessData = oldProcess->ProcessData;
	newProcess->ProcessId = oldProcess->ProcessId;
	strcpy(newProcess->ProcessName, oldProcess->ProcessName);
	newProcess->ListOfDlls = DLL_Head;
	free(oldProcess);
	return newProcess;
}

void CreateListOfProcessesFile(t_Process* Process_node)
{
	
	if (Process_Head == NULL)
	{
		Process_node->next = NULL;
		Process_node->prev = NULL;
		Process_Tail = Process_node;
		Process_Head = Process_node;
		
	}
	else
	{
		Process_Tail->next = Process_node;
		Process_node->prev = Process_Tail;
		Process_node->next = NULL;
		Process_Tail = Process_node;
		return NULL;
	}
}


t_SnapShot* SnapShotInfoFromFile(t_SnapShot* oldSample)
{
	t_SnapShot* newSample = (t_SnapShot*)malloc(sizeof(t_SnapShot));
	newSample->CountNumberOfProcessesInEachSnapShot = oldSample->CountNumberOfProcessesInEachSnapShot;
	newSample->CountNumberOfSnapShot = oldSample->CountNumberOfSnapShot;
	strcpy(newSample->TimeOfSnapShot, oldSample->TimeOfSnapShot);
	newSample->ListOfProcesses = Process_Head;
	free(oldSample);
	return newSample;
}

void CreateListOfSnapshots(t_SnapShot* newSample)
{
	if (SnapShot_Head == NULL)
	{
		newSample->next = NULL;
		newSample->prev = NULL;
		SnapShot_Head = newSample;
		SnapShot_Tail = newSample;
	}
	else
	{
		SnapShot_Tail->next = newSample;
		newSample->prev = SnapShot_Tail;
		newSample->next = NULL;
		SnapShot_Tail = newSample;
	}
}