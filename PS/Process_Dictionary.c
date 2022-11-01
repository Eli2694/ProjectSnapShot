#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#pragma warning(disable:4996)

//function Specifications 
void CreateListOfMonovalentProcessess(t_Process* SampleProcess);
int calculateNumOfMonovalentProcess();
unsigned long long calculateAvgOfAvgWorkingSetSize(unsigned int sumOfProcesses);

// Variable Declaration
t_Processes_Dictionary* Process_DictionaryHead = NULL;
t_Processes_Dictionary* Process_DictionaryTail = NULL;

t_Processes_Dictionary* ProcessTraversing(t_SnapShot* SnapShotHead)
{
	if (SnapShotHead == NULL)
	{
		return;
	}

	t_SnapShot* currentSnapShot = SnapShotHead;
	t_Process* currentProcess;

	while (currentSnapShot)
	{
		currentProcess = currentSnapShot->ListOfProcesses;
		while (currentProcess)
		{
			CreateListOfMonovalentProcessess(currentProcess);
			currentProcess = currentProcess->next;
		}
		currentSnapShot = currentSnapShot->next;
	}

	return Process_DictionaryHead;
}

void CreateListOfMonovalentProcessess(t_Process* SampleProcess)
{

	t_Processes_Dictionary* d_Process = (t_Processes_Dictionary*)malloc(sizeof(t_Processes_Dictionary));
	if (d_Process == NULL)
	{
		LogError("Allocation Memory Of d_Process");
		return;
	}

	strcpy(d_Process->Key_Process_Name, SampleProcess->ProcessName);
	d_Process->WorkingSetSize = SampleProcess->ProcessData.WorkingSetSize;
	d_Process->ProcessId = SampleProcess->ProcessId;
	d_Process->next = d_Process->prev = NULL;

	t_Processes_Dictionary* curr = Process_DictionaryHead;
	t_Processes_Dictionary* tempProcessHead = Process_DictionaryHead;

	if (curr == NULL)
	{
		Process_DictionaryHead = Process_DictionaryTail = d_Process;
	}
	else
	{
		while (curr)
		{
			//Comparing IDs between process from Sample and process from t_Processes_Dictionary linked list
			if (curr->ProcessId == d_Process->ProcessId)
			{
				free(d_Process);
				break;
			}
			if (curr->next == NULL)
			{
				t_Processes_Dictionary* newProcess = (t_Processes_Dictionary*)malloc(sizeof(t_Processes_Dictionary));
				if (newProcess == NULL)
				{
					LogError("Allocation Memory Of newProcess (Process_Dictionary)");
					return;
				}

				*newProcess = *d_Process;

				curr->next = newProcess;
				newProcess->prev = curr;
				curr = newProcess;
				newProcess->next = NULL;
				free(d_Process);
				break;
			}
			curr = curr->next;
		}
	}
	curr = tempProcessHead; // i need to always check all the dictionary list for similar processes
}

int calculateNumOfMonovalentProcess()
{
	unsigned int sumOfProcesses = 0;
	t_Processes_Dictionary* curr = Process_DictionaryHead;
	while (curr)
	{
		sumOfProcesses++;
		curr = curr->next;
	}
	return sumOfProcesses;
}

unsigned long long calculateAvgOfAvgWorkingSetSize(t_SnapShot* SnapShot_Head)
{
	unsigned int SumOfProcesses = 0;
	unsigned long long AvgWorkingSetSize = 0;
	t_SnapShot* currSnapshot = SnapShot_Head;
	t_Process* currProcess;
	while (currSnapshot)
	{
		currProcess = currSnapshot->ListOfProcesses;
		while (currProcess)
		{
			SumOfProcesses++;
			AvgWorkingSetSize = AvgWorkingSetSize + currProcess->ProcessData.WorkingSetSize;
			currProcess = currProcess->next;
			
		}
		
		currSnapshot = currSnapshot->next;
	}
	AvgWorkingSetSize = AvgWorkingSetSize / SumOfProcesses;
	return AvgWorkingSetSize;
}