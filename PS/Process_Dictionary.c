#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#pragma warning(disable:4996)

// Variable Declaration
t_Processes_Dictionary* Process_DictionaryHead = NULL;
t_Processes_Dictionary* Process_DictionaryTail = NULL;


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
		
			currentProcess = currentProcess->next;
		}
		currentSnapShot = currentSnapShot->next;
	}

	return Process_DictionaryHead;
}