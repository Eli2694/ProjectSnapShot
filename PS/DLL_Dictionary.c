//typedef struct s_DLL_Dictionary
//{
//	char Key_Dll_Name[MAX_PATH];
//	t_Process* Process_List;
//	struct s_DLL_Dictionary* next;
//	struct s_DLL_Dictionary* prev;
//}t_DLL_Dictionary;

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "SaveToFILE.h"
#include "Log.h"
#pragma warning(disable:4996)

//function Specification
void addToDLLDictionaryList(t_DLL_Dictionary* DLL);
void addProcessToDLL(t_Process* ProcessOfd_DLL);
void Dictionary(t_DLL* DLL, t_Process* Process);

//variable Declaration
t_DLL_Dictionary* DLL_DictionaryHead = NULL;
t_DLL_Dictionary* DLL_DictionaryTail = NULL;

t_DLL_Dictionary* TraversingSnapshot(t_SnapShot* TailOfSnapShot)
{
	if (TailOfSnapShot == NULL)
	{
		return;
	}

	t_SnapShot* HeadOfSnapShot = runToHeadOfSanpShot(TailOfSnapShot);
	t_SnapShot* currSnapShot = HeadOfSnapShot;
	t_Process* currProcess;
	t_DLL* currDLL;

	while (currSnapShot)
	{
		currProcess = currSnapShot->ListOfProcesses;
		while (currProcess)
		{
			currDLL = currProcess->ListOfDlls;
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
	d_DLL->Process_List->next = d_DLL->Process_List->prev = NULL;
	strcpy(d_DLL->Process_List->ProcessName, Process->ProcessName);
	d_DLL->Process_List->ProcessData.WorkingSetSize = Process->ProcessData.WorkingSetSize;

	// Variable that will help me traverse the list
	t_DLL_Dictionary* curr = DLL_DictionaryHead;

	if (curr == NULL)
	{
		addToDLLDictionaryList(d_DLL);
	}
	else
	{
		while (curr)
		{
			if (strcmp(curr->Key_Dll_Name, d_DLL->Key_Dll_Name) == 0)
			{
				addProcessToDLL(d_DLL->Process_List);
				break;
			}
			if (curr->next == NULL)
			{
				addToDLLDictionaryList(d_DLL);
			}

			curr = curr->next;
		}

	}

	
}

void addToDLLDictionaryList(t_DLL_Dictionary* DLL)
{
	if (DLL_DictionaryHead == NULL)
	{
		DLL_DictionaryHead = DLL;
		DLL_DictionaryTail = DLL;
		DLL->next = NULL;
		DLL->prev = NULL;
	}
	else
	{
		DLL_DictionaryTail->next = DLL;
		DLL->prev = DLL_DictionaryTail;
		DLL_DictionaryTail = DLL;
		DLL->next = NULL;
	}
}

void addProcessToDLL(t_Process* ProcessOfd_DLL)
{

}