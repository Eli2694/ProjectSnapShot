#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "GetProcessInfo.h"
#include "SnapShot.h"
#include "SaveToFILE.h"
#pragma warning(disable:4996)

// something do not work

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
		fread(SnapShot_Information, sizeof(t_SnapShot), 1, in);
		// Number of processes in each SnapShot
		for ( int i = 0; i < SnapShot_Information->CountNumberOfProcessesInEachSnapShot; i++)
		{
			Process_Information = (t_Process*)malloc(sizeof(t_Process));
			fread(Process_Information, sizeof(t_Process), 1, in);
			for ( int j = 0; j < Process_Information->NumberOfDLLsInEachProcess; j++)
			{
				DLL_Information = (t_DLL*)malloc(sizeof(t_DLL));
				fread(DLL_Information, sizeof(t_DLL), 1, in);
				CreateListOfDlls(DLL_Information);
			}

			Process_Information->ListOfDlls = DLL_Head;
			DLL_Head = NULL;
			DLL_Tail = NULL;

			CreateListOfProcessesFile(Process_Information);
		}

		SnapShot_Information->ListOfProcesses = Process_Head;
		CreateListOfSnapshots(SnapShot_Information);
	}
	fclose(in);

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
	}

	DLL_Head = NULL;
	DLL_Tail = NULL;
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