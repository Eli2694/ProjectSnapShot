#include <time.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "GetProcessInfo.h"
#include "Log.h"
#pragma warning(disable:4996)


//function - specifications
void addProcess(t_Process* fixed, t_Process* temp);
void addDLL(t_DLL* fixed, t_DLL* temp);
t_SnapShot* AggregationOfData(t_SnapShot* SnapShot_Tail, t_SnapShot* Sample);
void cleaningDistruptingProcess(t_SnapShot* HeadOfSnapShot);




t_SnapShot* AggregationOfData(t_SnapShot* SnapShot_Tail, t_SnapShot* Sample)
{
	t_SnapShot* headOfList = SnapShot_Tail;
	t_SnapShot* currS = SnapShot_Tail;
	t_SnapShot* tempS = Sample;

	t_Process* currProcesses;
	t_Process* tempProcesses;
	t_DLL* currDlls;
	t_DLL* tempDlls;
	while (tempS)
	{
		while (currS)
		{
			
			tempProcesses = tempS->ListOfProcesses;
			while (tempProcesses)
			{
				currProcesses = currS->ListOfProcesses;
				while (currProcesses)
				{
					if (tempProcesses->ProcessId == currProcesses->ProcessId)
					{
						//AggregationOfData
						currProcesses->ProcessData.PageFaultCount += tempProcesses->ProcessData.PageFaultCount;
						currProcesses->ProcessData.WorkingSetSize += tempProcesses->ProcessData.WorkingSetSize;
						currProcesses->ProcessData.PagefileUsage += tempProcesses->ProcessData.PagefileUsage;
						currProcesses->ProcessData.QuotaPagedPoolUsage += tempProcesses->ProcessData.QuotaPagedPoolUsage;
						currProcesses->ProcessData.QuotaPeakPagedPoolUsage += tempProcesses->ProcessData.QuotaPeakPagedPoolUsage;

						tempDlls = tempProcesses->ListOfDlls;
						while (tempDlls)
						{
							currDlls = currProcesses->ListOfDlls;
							while (currDlls)
							{
								if (strcmp(tempDlls->NameOfDLL, currDlls->NameOfDLL) == 0)
								{
									break;
								}
								if (currDlls->next == NULL)
								{
									currProcesses->NumberOfDLLsInEachProcess++;
									addDLL(currDlls, tempDlls);
									break;
								}

								currDlls = currDlls->next;
							}

							tempDlls = tempDlls->next;
						}

						break;
					}
					// Identical processes were not found
					// Add new Process
					if (currProcesses->next == NULL)
					{
						currS->CountNumberOfProcessesInEachSnapShot++;
						addProcess(currProcesses, tempProcesses); // problam
						break; // to change tempProcesses
					}

					currProcesses = currProcesses->next;
				}
				tempProcesses = tempProcesses->next;
			}
			
			currS = currS->next;
		}

		// Every time i compare temp variable with the whole list of fixed SnapShot
		currS = headOfList;

		tempS = tempS->next;
	}

	return SnapShot_Tail;

}

void addProcess(t_Process* curr, t_Process* temp)
{ 
	t_Process* cProcess = curr;
	t_Process* tProcess = temp;

	//Allocate memory to a new variable so as not to disrupt the list it is in
	t_Process* newCurrProcess = (t_Process*)malloc(sizeof(t_Process));
	if (newCurrProcess == NULL)
	{
		LogError("Allocation memory Program (addProcess)");
		exit(1);
	}
	newCurrProcess->ListOfDlls = (t_DLL*)malloc(sizeof(t_DLL));
	if (newCurrProcess->ListOfDlls == NULL)
	{
		LogError("Allocation memory Program (addProcess)");
		exit(1);
	}

	strcpy(newCurrProcess->ProcessName, tProcess->ProcessName);
	newCurrProcess->ProcessData = tProcess->ProcessData;
	newCurrProcess->ProcessId = tProcess->ProcessId;
	newCurrProcess->NumberOfDLLsInEachProcess = tProcess->NumberOfDLLsInEachProcess;
	newCurrProcess->ListOfDlls = tProcess->ListOfDlls;
	newCurrProcess->next = tProcess->next;
	newCurrProcess->prev = tProcess->prev;

	cProcess->next = newCurrProcess;
	newCurrProcess->prev = cProcess;
	newCurrProcess->next = NULL;

	// I need to keep the list of DLLS in newCurrProcess
	temp->ListOfDlls = NULL;

}

void addDLL(t_DLL* curr, t_DLL*temp)
{ 

	t_DLL* cDLL = curr;
	t_DLL* tDLL = temp;

	//Allocate memory to a new variable so as not to disrupt the list it is in
	t_DLL* newCurrDLL = (t_DLL*)malloc(sizeof(t_DLL));
	if (newCurrDLL == NULL)
	{
		LogError("Allocation memory Program (addDLL)");
		exit(1);
	}

	strcpy(newCurrDLL->NameOfDLL, tDLL->NameOfDLL);
	newCurrDLL->next = tDLL->next;
	newCurrDLL->prev = tDLL->prev;

	cDLL->next = newCurrDLL;
	newCurrDLL->prev = cDLL;
	newCurrDLL->next = NULL;
}

//Preventing non-routine processes from harming the program
void cleaningDistruptingProcess(t_SnapShot*HeadOfSnapShot)
{
	t_SnapShot* curr = HeadOfSnapShot;
	t_Process* currProcess = curr->ListOfProcesses;

	while (curr)
	{
		while (currProcess)
		{
			if (currProcess->ProcessId > 100000)
			{
				currProcess->next = NULL;
				currProcess->ListOfDlls = NULL;
				currProcess->prev->next = NULL;
				free(currProcess);

			}

			currProcess = currProcess->next;
		}

		curr = curr->next;
	}

}