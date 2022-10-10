#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#pragma warning(disable:4996)

//function Specifications
void CreateListOfMonovalentProcessess(t_Process* SampleProcess);
int calculateNumOfMonovalentProcess();
void calculateAvgOfAvgWorkingSetSize(unsigned int sumOfProcesses);

// Variable Declaration
t_Processes_Dictionary* Process_DictionaryHead = NULL;
t_Processes_Dictionary* Process_DictionaryTail = NULL;

unsigned long long AvgWorkingSetSize = 0;
unsigned int sumOfProcesses = 0;


t_Processes_Dictionary* ProcessTraversing(t_SnapShot* SnapShotP)
{
	if (SnapShotP == NULL)
	{
		return;
	}

	t_SnapShot* currentSnapShot = SnapShotP;
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
	d_Process->next = d_Process->prev = NULL;

	t_Processes_Dictionary* curr = Process_DictionaryHead;
	if (curr == NULL)
	{

		Process_DictionaryHead = Process_DictionaryTail = d_Process;
	}
	else
	{
		while (curr)
		{
			if (strcmp(curr->Key_Process_Name, d_Process->Key_Process_Name) == 0)
			{
				free(d_Process);
				break;
			}
			if (curr->next == NULL)
			{
				curr->next = d_Process;
				d_Process->prev = curr;
				curr = d_Process;
				d_Process->next = NULL;
				break;
			}
			curr = curr->next;
		}
	}
	curr = Process_DictionaryHead;
}

int calculateNumOfMonovalentProcess()
{
	
	t_Processes_Dictionary* curr = Process_DictionaryHead;
	while (curr)
	{
		sumOfProcesses++;
		curr = curr->next;
	}
	return sumOfProcesses;
}

void calculateAvgOfAvgWorkingSetSize(unsigned int sumOfProcesses)
{
	unsigned long long SumWorkingSetSize = 0;
	t_Processes_Dictionary* curr = Process_DictionaryHead;
	while (curr)
	{
		SumWorkingSetSize = SumWorkingSetSize + curr->WorkingSetSize;
		curr = curr->next;
	}
	AvgWorkingSetSize = SumWorkingSetSize / sumOfProcesses;
}