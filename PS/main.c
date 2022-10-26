#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <conio.h>
#include "Log.h"
#include "Struct.h"
#include "GetProcessInfo.h"
#include "SnapShot.h"
#include "SaveToFILE.h"
#include "LoadFromFile.h"
#include "AggregationOfData.h"
#include "resetCollection.h"
#include "DLL_Dictionary.h"
#include "BubbleSort.h"
#include "Process_Dictionary.h"
#include "GenerateHTML_DLLs_Dictionary.h"
#include "GenerateHTML_SnapShot.h"
#include "GenerateHTML_DLLsProcesses.h"
#pragma warning(disable:4996)

//Variable Declaration
t_SnapShot* smaple;
t_SnapShot* tailSnapShot;
t_DLL_Dictionary* DLL_Dictionary_List;
t_Processes_Dictionary* Process_Dictionary_List;

int main()
{
	int stop = 0;
	int user_response = -1;
	char str[10];

	

	do {
		printf("<--User-Response-->\n1.Take One SnapShot\n2.Take 20 SnapShots\n3.Start Long SnapShot\n4.Generate HTML Report\n5.Reset Collections\n6.Save In File\n7.Load From File\n8.Quit\n");
		scanf("%d", &user_response);

		switch (user_response)
		{
		case 1:
			LogEvent("Creating one sample");
			smaple = GetProcessesInfo(); // Creating a sample
			LogEvent("Creating list of snapshots");
			tailSnapShot = ListOfSnapShots(smaple); // Inserting into a list of samples
			break;
		case 2:
			smaple = GetProcessesInfo();
			tailSnapShot = ListOfSnapShots(smaple);
			LogEvent("Create one sample with data accumulation for at least 20 seconds");
			for (int i = 0; i < 19; i++)
			{
				smaple = GetProcessesInfo();
				tailSnapShot = AggregationOfData(tailSnapShot, smaple);
				freeSample(smaple);
				Sleep(1000);
			}
			break;
		case 3:
			LogEvent("Creating a single sample with data accumulation until the user decides to stop");
			smaple = GetProcessesInfo();
			tailSnapShot = ListOfSnapShots(smaple);
			while (stop != 1)
			{
				smaple = GetProcessesInfo();
				tailSnapShot = AggregationOfData(tailSnapShot, smaple);
				freeSample(smaple);
				Sleep(1000);

				if (kbhit())
				{
					printf("Enter 'END' To Stop The Loop\n");
					scanf("%s", str);
					if (strcmp(str, "END") == 0)
					{
						stop = 1;
					}
				}
			}
			break;
		case 4:
			LogEvent("Creating Dictionary List Of DLLs");
			DLL_Dictionary_List = SnapshotTraversing(SnapShot_Head); // monovalent list 
			LogEvent("Creating Dictionary List Of Processes");
			Process_Dictionary_List = ProcessTraversing(SnapShot_Head); // monovalent list 
			sort(tailSnapShot); // Bubble sort
			LogEvent("Generating HTML Files");
			CreateProjectPage();
			createDLLsProcessListInHTML(); // including list of processe using the Dlls
			createsnapShotLisInHTML();
			break;
		case 5:
			LogEvent("Release linked lists and freeing memory");
			FreeSnapShotList(tailSnapShot);
			FreeDictionaryListOfDLLs(DLL_Dictionary_List);
			FreeDictionaryListOfProcesses(Process_Dictionary_List);
			break;
		case 6:
			LogEvent("Writing linked-list to binary files");
			WriteToBinaryFile(tailSnapShot);
			break;
		case 7:
			LogEvent("Reading linked-list from binary files");
			LoadFromFile();
			break;
		case 8:
			LogEvent("Exit Program");
			user_response = 8;
			break;
		}
	} while (user_response != 8);


	return 0;
}