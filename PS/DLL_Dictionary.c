#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "SaveToFILE.h"
#include "Log.h"
#pragma warning(disable:4996)

//function Specification
void addToDLLDictionaryList(t_DLL_Dictionary* DLL);
void Dictionary(t_DLL* DLL, t_Process* Process);
void addProcessToDLL(t_Process* currDLL, t_Process* d_DLL);
void calculateNumOfProcessInDll();
unsigned int calculateSumOfDLLs();

//variable Declaration
t_DLL_Dictionary* DLL_DictionaryHead = NULL;
t_DLL_Dictionary* DLL_DictionaryTail = NULL;

t_DLL_Dictionary* SnapshotTraversing(t_SnapShot* SnapShot)
{
	if (SnapShot == NULL)
	{
		return;
	}

	//t_SnapShot* HeadOfSnapShot = runToHeadOfSanpShot(TailOfSnapShot);
	t_SnapShot* currSnapShot = SnapShot;
	t_Process* currProcess;
	t_DLL* currDLL;

	while (currSnapShot)
	{
		currProcess = currSnapShot->ListOfProcesses;
		while (currProcess)
		{
			currDLL = currSnapShot->ListOfProcesses->ListOfDlls;
			while (currDLL)
			{
				Dictionary(currDLL, currProcess);
				currDLL = currDLL->next;
			}

			currProcess = currProcess->next;
		}
		currSnapShot = currSnapShot->next;
	}
	
	return DLL_DictionaryHead;
}

void Dictionary(t_DLL* DLL, t_Process* Process)
{
	t_DLL_Dictionary* d_DLL = (t_DLL_Dictionary*)malloc(sizeof(t_DLL_Dictionary));
	if (!d_DLL)
	{
		LogError("Allocation Memory Of t_DLL_Dictionary");
		return;
	}
	//DLL info
	strcpy(d_DLL->Key_Dll_Name, DLL->NameOfDLL);
	d_DLL->next = d_DLL->prev = NULL;
	
	d_DLL->Process_List = (t_Process*)malloc(sizeof(t_Process));
	if (!d_DLL->Process_List)
	{
		LogError("Allocation Memory Of t_DLL_Dictionary -> Process_List");
		return;
	}
	//Process info inside DLL_Dictionary Structure
	strcpy(d_DLL->Process_List->ProcessName, Process->ProcessName);
	d_DLL->Process_List->ProcessData = Process->ProcessData;
	d_DLL->Process_List->ProcessId = Process->ProcessId;
	d_DLL->NumOfProcess = 1;
	d_DLL->Process_List->next = d_DLL->Process_List->prev = NULL;

	// Variable that will help me traverse the list
	t_DLL_Dictionary* curr = DLL_DictionaryHead;

	if (curr == NULL)
	{
		t_DLL_Dictionary* h_DLL = (t_DLL_Dictionary*)malloc(sizeof(t_DLL_Dictionary));
		h_DLL->Process_List = (t_Process*)malloc(sizeof(t_Process));

		strcpy(h_DLL->Key_Dll_Name, d_DLL->Key_Dll_Name);
		strcpy(h_DLL->Process_List->ProcessName, d_DLL->Process_List->ProcessName);
		h_DLL->Process_List->ProcessData = d_DLL->Process_List->ProcessData;
		h_DLL->Process_List->ProcessId = d_DLL->Process_List->ProcessId;
		h_DLL->NumOfProcess = d_DLL->NumOfProcess;
		h_DLL->next = h_DLL->prev = NULL;
		h_DLL->Process_List->next = h_DLL->Process_List->prev = NULL;

		addToDLLDictionaryList(h_DLL);
		free(d_DLL->Process_List);
		free(d_DLL);
	}
	else
	{
		while (curr)
		{
			if (strcmp(curr->Key_Dll_Name, d_DLL->Key_Dll_Name) == 0)
			{
				addProcessToDLL(curr->Process_List,d_DLL->Process_List);
				break;
			}
			if (curr->next == NULL)
			{
				t_DLL_Dictionary* n_DLL = (t_DLL_Dictionary*)malloc(sizeof(t_DLL_Dictionary));
				n_DLL->Process_List = (t_Process*)malloc(sizeof(t_Process));

				strcpy(n_DLL->Key_Dll_Name, d_DLL->Key_Dll_Name);
				strcpy(n_DLL->Process_List->ProcessName, d_DLL->Process_List->ProcessName);
				n_DLL->Process_List->ProcessData = d_DLL->Process_List->ProcessData;
				n_DLL->Process_List->ProcessId = d_DLL->Process_List->ProcessId;
				n_DLL->NumOfProcess = d_DLL->NumOfProcess;
				n_DLL->next = n_DLL->prev = NULL;
				n_DLL->Process_List->next = n_DLL->Process_List->prev = NULL;

				addToDLLDictionaryList(n_DLL);
				break;
			}
			curr = curr->next;
		}

		free(d_DLL->Process_List);
		free(d_DLL);

	}

}

void addToDLLDictionaryList(t_DLL_Dictionary* j_DLL)
{
	if (DLL_DictionaryHead == NULL)
	{
		j_DLL->next = NULL;
		j_DLL->prev = NULL;
		DLL_DictionaryHead = j_DLL;
		DLL_DictionaryTail = j_DLL;

	}
	else
	{
		DLL_DictionaryTail->next = j_DLL;
		j_DLL->prev = DLL_DictionaryTail;
		j_DLL->next = NULL;
		DLL_DictionaryTail = j_DLL;
	}
}

void addProcessToDLL(t_Process* currDLL, t_Process* d_DLL)
{
	t_Process* currDictionaryDLL = currDLL;
	
	while (currDictionaryDLL)
	{
		if (strcmp(currDictionaryDLL->ProcessName, d_DLL->ProcessName) == 0)
		{
			break;
		}

		if (currDictionaryDLL->next == NULL)
		{
			t_Process* new_process = (t_Process*)malloc(sizeof(t_Process));
			strcpy(new_process->ProcessName, d_DLL->ProcessName);
			new_process->ProcessData = d_DLL->ProcessData;
			new_process->ProcessId = d_DLL->ProcessId;
			

			//Insetring Into List Of Processes
			currDictionaryDLL->next = new_process;
			new_process->prev = currDictionaryDLL;
			new_process->next = NULL;
			
			break;
		}
		currDictionaryDLL = currDictionaryDLL->next;
	}
}

void calculateNumOfProcessInDll()
{
	t_DLL_Dictionary* curr = DLL_DictionaryHead;
	t_Process* currProcesses;
	while (curr)
	{
		curr->NumOfProcess = 1;
		currProcesses = curr->Process_List;
		while (currProcesses)
		{
			curr->NumOfProcess++;
			currProcesses = currProcesses->next;
		}

		curr = curr->next;
	}
}

unsigned int calculateSumOfDLLs()
{
	unsigned int SumOfDLLs = 0;
	t_DLL_Dictionary* curr = DLL_DictionaryHead;
	while (curr)
	{
		SumOfDLLs++;
		curr = curr->next;
	}
	return SumOfDLLs;
}