#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "GetProcessInfo.h"
#include "SnapShot.h"
#include "DLL_Dictionary.h"
#include "Process_Dictionary.h"

//function specification
void FreeDictionaryListOfDLLs(t_DLL_Dictionary* HeadOfDictionaryList);
void FreeDictionaryListOfProcesses(t_Processes_Dictionary* HeadOfDictionaryProcessList);


void FreeSnapShotList(t_SnapShot*EndOfSnapShots)
{
	if (EndOfSnapShots == NULL)
	{
		return;
	}

	t_SnapShot* currSample = EndOfSnapShots;
	t_SnapShot* releaseSample;
	t_Process* releaseProcess;
	t_DLL* releaseDLL;
	while (currSample)
	{
		// free SnapShot
		releaseSample = currSample;
		while (currSample->ListOfProcesses)
		{
			releaseProcess = currSample->ListOfProcesses;
			// free Processes
			while (currSample->ListOfProcesses->ListOfDlls)
			{
				//free DLLs
				releaseDLL = currSample->ListOfProcesses->ListOfDlls;
				currSample->ListOfProcesses->ListOfDlls = currSample->ListOfProcesses->ListOfDlls->next;
				free(releaseDLL);
			}

			
			currSample->ListOfProcesses = currSample->ListOfProcesses->next;
			free(releaseProcess);
		}

		currSample = currSample->prev;
		free(releaseSample);
	}
	SnapShot_Head = NULL;
	DLL_Head = NULL;
	DLL_Tail = NULL;
	Process_Head = NULL;
	Process_Tail = NULL;
}

void FreeDictionaryListOfDLLs(t_DLL_Dictionary* HeadOfDictionaryList)
{
	if (HeadOfDictionaryList == NULL)
	{
		return;
	}

	t_DLL_Dictionary* currDLL = HeadOfDictionaryList;
	t_DLL_Dictionary* releaseCurrDLL;
	t_Process* currProcess;
	t_Process* releaseCurrProcess;

	while (currDLL)
	{

		currProcess = currDLL->Process_List;
		while (currProcess)
		{
			releaseCurrProcess = currProcess;
			currProcess = currProcess->next;
			free(releaseCurrProcess);
		}
		releaseCurrDLL = currDLL;
		currDLL = currDLL->next;
		free(releaseCurrDLL);
	}
}

void FreeDictionaryListOfProcesses(t_Processes_Dictionary* HeadOfDictionaryProcessList)
{
	if (HeadOfDictionaryProcessList == NULL)
	{
		return;
	}

	t_Processes_Dictionary* currProcess = HeadOfDictionaryProcessList;
	t_Processes_Dictionary* release;
	while (currProcess)
	{
		release = currProcess;
		currProcess = currProcess->next;
		free(release);
	}
	
}