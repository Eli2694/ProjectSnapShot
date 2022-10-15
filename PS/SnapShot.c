#include <time.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "GetProcessInfo.h"
#pragma warning(disable:4996)

//function - specifications
void freeSample(t_SnapShot* sample);
int CountNumberOfProcess(t_Process* ListOfProcesses);
int CountNumberOfSnapShots(t_SnapShot* ListOfSnapShots);

//variable declaration
t_SnapShot* SnapShot_Head = NULL;
t_SnapShot* SnapShot_Tail = NULL;

t_SnapShot* OneSnapShot(t_Process* HeadOfProcessList)
{
	//time
	char CurrentTimeOfSnapShot[40];
	time_t t;
	time(&t);
	struct tm* timeInfo;
	timeInfo = localtime(&t);
	sprintf(CurrentTimeOfSnapShot, "%d/%d/%d - %02d:%02d:%02d", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

	t_SnapShot* sample;
	sample = (t_SnapShot*)malloc(sizeof(t_SnapShot));
	sample->ListOfProcesses = HeadOfProcessList;
	strcpy(sample->TimeOfSnapShot, CurrentTimeOfSnapShot);
	sample->next = sample->prev = NULL;

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

int CountNumberOfSnapShots(t_SnapShot* ListOfSnapShots)
{
	t_SnapShot* curr = ListOfSnapShots;
	int countSnapShots = 0;
	while (curr)
	{
		countSnapShots++;
		curr = curr->next;
	}
	return countSnapShots;
}
