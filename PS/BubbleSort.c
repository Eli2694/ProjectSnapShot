#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "GetProcessInfo.h"
#include "SaveToFILE.h"
#include "Log.h"
#pragma warning(disable:4996)

//function specification
void replace(t_Process* currProcess);

//global Variable
t_SnapShot* currS;

void sort(t_SnapShot* TailOfSnapShot)
{
	if (TailOfSnapShot == NULL)
	{
		return;
	}
	t_SnapShot* HSnapShot = runToHeadOfSanpShot(TailOfSnapShot);
	t_SnapShot* currS = HSnapShot;
	t_Process* currP;
	t_DLL* currD;

	while (currS)
	{
		currP = currS->ListOfProcesses;
		while (currP)
		{
			
			if (currP->NumberOfDLLsInEachProcess > currP->next->NumberOfDLLsInEachProcess)
			{
				replace(currP);
			}

			currP = currP->next;
		}

		currS = currS->next;
	}	

}

void replace(t_Process* currProcess)
{
	t_Process* temp1 = currProcess;
	t_Process* temp2 = currProcess->next;

	if (currProcess == NULL)
	{
		return;
	}
	else if (temp1->prev == NULL && temp2->next == NULL) // 2 Processes
	{
		temp2->next = temp1;
		temp1->prev = temp2;
		temp1->next = NULL;
		temp2->prev = NULL;
		currS->ListOfProcesses = temp2;
	}
	else if (temp1->prev != NULL && temp2->next == NULL) // before the last one
	{
		temp1->prev->next = temp2;
		temp2->next = temp1;
		temp2->prev = temp1->prev;
		temp1->next = NULL;
		temp1->prev = temp2;
	}
	else if (temp1->prev == NULL && temp2->next != NULL)
	{
		temp2->next->prev = temp1;
		temp1->next = temp2->next;
		temp1->prev = temp2;
		temp2->next = temp1;
		temp2->prev = NULL;
		currS->ListOfProcesses = temp2;
	}
	else
	{
		temp2->next->prev = temp1;
		temp1->prev->next = temp2;
		temp2->prev = temp1->prev;
		temp1->next = temp2->next;
		temp2->next = temp1;
		temp1->prev = temp2;
	}

		

}


