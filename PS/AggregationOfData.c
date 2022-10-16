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
	t_Process* saveSnapShotAddress = SnapShot_Tail->ListOfProcesses;
	t_Process* fixed = SnapShot_Tail->ListOfProcesses;
	t_Process* temp = Sample->ListOfProcesses;
	t_DLL* fixedProcessDLLs;
	t_DLL* tempProcessDlls;
	while (temp)
	{
		while (fixed)
		{
			// Checking if the ID of processes from different lists is the same
			if (temp->ProcessId == fixed->ProcessId)
			{
				//AggregationOfData
				fixed->ProcessData.PageFaultCount += temp->ProcessData.PageFaultCount;
				fixed->ProcessData.WorkingSetSize += temp->ProcessData.WorkingSetSize;
				fixed->ProcessData.PagefileUsage += temp->ProcessData.PagefileUsage;
				fixed->ProcessData.QuotaPagedPoolUsage += temp->ProcessData.QuotaPagedPoolUsage;
				fixed->ProcessData.QuotaPeakPagedPoolUsage += temp->ProcessData.QuotaPeakPagedPoolUsage;

				tempProcessDlls = temp->ListOfDlls;
				while (tempProcessDlls)
				{
					fixedProcessDLLs = fixed->ListOfDlls;

					while (fixedProcessDLLs)
					{
						//Checking if the Name of DLLs from different lists is the same
						if (strcmp(fixedProcessDLLs->NameOfDLL, temp->ListOfDlls->NameOfDLL) == 0)
						{
							break;
						}
						if (fixedProcessDLLs->next == NULL)
						{
							//Adding DLL To a fixedProcessDLLs
							addDLL(fixedProcessDLLs, tempProcessDlls);
							fixed->NumberOfDLLsInEachProcess++;
							break;
							
						}
						fixedProcessDLLs = fixedProcessDLLs->next;
					}
					tempProcessDlls = tempProcessDlls->next;

				}
				break;
			}
			if (fixed->next == NULL)
			{
				// We reached the last member and did not find a match between Processes
				addProcess(fixed, temp);
				SnapShot_Tail->CountNumberOfProcessesInEachSnapShot++;
				break;
			}
			fixed = fixed->next;
			
		}
		// Every time i compare temp variable with the whole list of fixed SnapShot
		fixed = saveSnapShotAddress;
		temp = temp->next;
	}
	return SnapShot_Tail;
}

void addProcess(t_Process* fixed, t_Process* temp)
{ 
	if (temp->ProcessId > 100000)
	{
		temp->prev->next = NULL;
		free(temp);
		return;
	}

	//Allocate memory to a new variable so as not to disrupt the list it is in
	t_Process* newFixedProcess = (t_Process*)malloc(sizeof(t_Process));
	if (newFixedProcess == NULL)
	{
		LogError("Allocation memory Program (addProcess)");
		exit(1);
	}
	newFixedProcess = temp;
	t_Process* fixedProcess = fixed;
	fixedProcess->next = newFixedProcess;
	newFixedProcess->prev = fixedProcess;
	newFixedProcess->next = NULL;

}

void addDLL(t_DLL* fixed, t_DLL* temp)
{ 
	//Allocate memory to a new variable so as not to disrupt the list it is in
	t_DLL* newFixeDLL = (t_DLL*)malloc(sizeof(t_DLL));
	if (newFixeDLL == NULL)
	{
		LogError("Allocation memory Program (addDLL)");
		exit(1);
	}
	newFixeDLL = temp;
	t_DLL* fixedDLL = fixed;
	fixedDLL->next = newFixeDLL;
	newFixeDLL->prev = fixedDLL;
	newFixeDLL->next = NULL;
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