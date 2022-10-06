#include <time.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "GetProcessInfo.h"
#pragma warning(disable:4996)

//function - specifications
void addProcess(t_Process* fixed, t_Process* temp);
void addDLL(t_DLL* fixed, t_DLL* temp);
void freeSample(t_SnapShot* sample);
int CountNumberOfProcess(t_Process* ListOfProcesses);


t_SnapShot* SnapShot_Head = NULL;
t_SnapShot* SnapShot_Tail = NULL;

t_SnapShot* OneSnapShot(t_Process* HeadOfProcessList)
{
	//time
	char CurrentTimeOfSnapShot[100];
	time_t t;
	time(&t);
	struct tm* timeInfo;
	timeInfo = localtime(&t);
	sprintf(CurrentTimeOfSnapShot, "%d/%d/%d - %02d:%02d:%02d", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

	t_SnapShot* sample;
	sample = (t_SnapShot*)malloc(sizeof(t_SnapShot));
	sample->ListOfProcesses = HeadOfProcessList;
	strcpy(sample->TimeOfSnapShot, CurrentTimeOfSnapShot);
	sample->CountNumberOfProcessesInEachSnapShot = CountNumberOfProcess(sample->ListOfProcesses);

	return sample;	
}

t_SnapShot* ListOfSnapShots(t_SnapShot* OneSnapShot)
{
	if (SnapShot_Head == NULL)
	{
		OneSnapShot->next = NULL;
		OneSnapShot->prev = NULL;
		SnapShot_Head = OneSnapShot;
		SnapShot_Tail = OneSnapShot;
		OneSnapShot->CountNumberOfSnapShot = 1;
	}
	else
	{
		OneSnapShot->CountNumberOfSnapShot = SnapShot_Tail->CountNumberOfSnapShot + 1;
		SnapShot_Tail->next = OneSnapShot;
		OneSnapShot->prev = SnapShot_Tail;
		OneSnapShot->next = NULL;
		SnapShot_Tail = OneSnapShot;
	}
	SnapShot_Tail->CountNumberOfProcessesInEachSnapShot = CountNumberOfProcess(OneSnapShot->ListOfProcesses);
	return SnapShot_Tail;
}


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

				t_DLL* tempProcessDlls = temp->ListOfDlls;
				while (tempProcessDlls)
				{
					t_DLL* currProcessDLLs = fixed->ListOfDlls;
					
					while (currProcessDLLs)
					{
						if (strcmp(currProcessDLLs->NameOfDLL, temp->ListOfDlls->NameOfDLL) == 0)
						{
							break;
						}
						if (currProcessDLLs->next == NULL)
						{
							addDLL(currProcessDLLs, tempProcessDlls);
							fixed->NumberOfDLLsInEachProcess++;
						}
						currProcessDLLs = currProcessDLLs->next;
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
			}
			fixed = fixed->next;
		}
		fixed = SnapShot_Tail->ListOfProcesses;
		temp = temp->next;
	}
	return SnapShot_Tail;
}

void addProcess(t_Process*fixed, t_Process*temp)
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

void freeSample(t_SnapShot* sample)
{
	t_SnapShot* currSample = sample;
	t_SnapShot* releaseSample;
	t_Process* releaseProcess;
	t_DLL* releaseDLL;

	while (currSample)
	{
		while (currSample->ListOfProcesses)
		{
			while (currSample->ListOfProcesses->ListOfDlls)
			{
				releaseDLL = currSample->ListOfProcesses->ListOfDlls;
				currSample->ListOfProcesses->ListOfDlls = currSample->ListOfProcesses->ListOfDlls->next;
				free(releaseDLL);
			}
			releaseProcess = currSample->ListOfProcesses;
			currSample->ListOfProcesses = currSample->ListOfProcesses->next;
			free(releaseProcess);
		}

		releaseSample = currSample;
		free(releaseSample);
		currSample = NULL;
	}
}

int CountNumberOfProcess(t_Process*ListOfProcesses)
{
	t_Process* curr = ListOfProcesses;
	int countProcesse = 0;
	while (curr)
	{
		countProcesse++;
		curr = curr->next;
	}
	return countProcesse;
}
