#include <time.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "GetProcessInfo.h"
#pragma warning(disable:4996)


//function - specifications
void addProcess(t_Process* fixed, t_Process* temp);
void addDLL(t_DLL* fixed, t_DLL* temp);
t_SnapShot* AggregationOfData(t_SnapShot* SnapShot_Tail, t_SnapShot* Sample);


t_SnapShot* AggregationOfData(t_SnapShot* SnapShot_Tail, t_SnapShot* Sample)
{
	t_Process* fixed = SnapShot_Tail->ListOfProcesses;
	t_Process* temp = Sample->ListOfProcesses;
	while (temp)
	{
		while (fixed)
		{
			if (temp->ProcessId == fixed->ProcessId && strcmp(temp->ProcessName, fixed->ProcessName) == 0)
			{
				fixed->ProcessData.PageFaultCount += temp->ProcessData.PageFaultCount;
				fixed->ProcessData.WorkingSetSize += temp->ProcessData.WorkingSetSize;
				fixed->ProcessData.PagefileUsage += temp->ProcessData.PagefileUsage;
				fixed->ProcessData.QuotaPagedPoolUsage += temp->ProcessData.QuotaPagedPoolUsage;
				fixed->ProcessData.QuotaPeakPagedPoolUsage += temp->ProcessData.QuotaPeakPagedPoolUsage;

				/*t_DLL* tempProcessDlls = temp->ListOfDlls;
				while (tempProcessDlls)
				///*{
				//	t_DLL* currProcessDLLs = fixed->ListOfDlls;

				//	while (currProcessDLLs)
				//	{
				//		if (strcmp(currProcessDLLs->NameOfDLL, temp->ListOfDlls->NameOfDLL) == 0)
				//		{
				//			break;
				//		}
				//		if (currProcessDLLs->next == NULL)
				//		{
				//			addDLL(currProcessDLLs, tempProcessDlls);
				//			fixed->NumberOfDLLsInEachProcess++;
				//		}
				//		currProcessDLLs = currProcessDLLs->next;
				//	}
				//	tempProcessDlls = tempProcessDlls->next;*/

				//}*/
				break;
			}
			//if (fixed->next == NULL)
			//{
			//	// We reached the last member and did not find a match between Processes
			//	addProcess(fixed, temp);
			//	SnapShot_Tail->CountNumberOfProcessesInEachSnapShot++;
			//	break;
			//}
			fixed = fixed->next;
		}
		fixed = SnapShot_Tail->ListOfProcesses;
		temp = temp->next;
	}
	return SnapShot_Tail;
}

void addProcess(t_Process* fixed, t_Process* temp)
{
	t_Process* lastProcess = fixed;
	t_Process* addSampleProcess = temp;
	lastProcess->next = addSampleProcess;
	addSampleProcess->prev = lastProcess;
	addSampleProcess->next = NULL;
}

void addDLL(t_DLL* fixed, t_DLL* temp)
{
	t_DLL* lastDLL = fixed;
	t_DLL* addSampleDLL = temp;
	lastDLL->next = addSampleDLL;
	addSampleDLL->prev = lastDLL;
	addSampleDLL->next = NULL;

}