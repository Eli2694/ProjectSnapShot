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
void CreateListOfSnapshots(t_SnapShot* newSample);
void CreateListOfProcessesFile(t_Process* Process_node);

t_SnapShot* LoadFromFile()
{
	FILE* in = fopen("SnapShot.bin", "rb");
	if (!in)
	{
		LogError("Problam Opening a file (SnapShot.bin)",strerror(GetLastError()));
		exit(1);
	}

	int read;
	t_HeaderFile LoadHeaderFile;
	read = fread(&LoadHeaderFile, sizeof(t_HeaderFile), 1, in);
	if (!read)
	{
		LogError("Problem reading from a binary file (LoadHeaderFile)");
		fclose(in);
		exit(1);
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
		if (!SnapShot_Information)
		{
			LogError("Memory Allocation Problam (SnapShot_Information)", strerror(GetLastError()));
			fclose(in);
			exit(1);
		}

		read = fread(SnapShot_Information, sizeof(t_SnapShot), 1, in);
		if (!read)
		{
			LogError("Problem reading from a binary file (SnapShot_Information)");
			fclose(in);
			exit(1);
		}


		// Number of processes in each SnapShot
		for ( int i = 0; i < SnapShot_Information->CountNumberOfProcessesInEachSnapShot; i++)
		{
			Process_Information = (t_Process*)malloc(sizeof(t_Process));
			if (!Process_Information)
			{
				LogError("Memory Allocation Problam (Process_Information)", strerror(GetLastError()));
				fclose(in);
				exit(1);
			}

			read = fread(Process_Information, sizeof(t_Process), 1, in);
			if (!read)
			{
				LogError("Problem reading from a binary file (Process_Information)");
				fclose(in);
				exit(1);
			}

			for (int j = 0; j < Process_Information->NumberOfDLLsInEachProcess; j++)
			{
				DLL_Information = (t_DLL*)malloc(sizeof(t_DLL));
				if (!DLL_Information)
				{
					LogError("Memory Allocation Problam (DLL_Information)", strerror(GetLastError()));
					fclose(in);
					exit(1);
				}

				read = fread(DLL_Information, sizeof(t_DLL), 1, in);
				if (!read)
				{
					LogError("Problem reading from a binary file (DLL_Information)");
					fclose(in);
					exit(1);
				}

				CreateListOfDlls(DLL_Information);
			}

			// because i need to free DLL_Head and DLL_Tail to create new list, i need to store the current list in a new variable
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