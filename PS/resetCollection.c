#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "GetProcessInfo.h"
#include "SnapShot.h"


void FreeMemoryAllocation(t_SnapShot*EndOfSnapShots)
{
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