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

	LogEvent("User_Response");
	do {
		printf("<--User-Response-->\n1.Take One SnapShot\n2.Take 20 SnapShots\n3.Start Long SnapShot\n4.Generate HTML Report\n5.Reset Collections\n6.Save In File\n7.Load From File\n8.Quit\n");
		scanf("%d", &user_response);

		switch (user_response)
		{
		case 1:
			smaple = GetProcessesInfo();
			tailSnapShot = ListOfSnapShots(smaple);
			break;
		case 2:
			smaple = GetProcessesInfo();
			tailSnapShot = ListOfSnapShots(smaple);
			for (int i = 0; i < 19; i++)
			{
				smaple = GetProcessesInfo();
				tailSnapShot = AggregationOfData(tailSnapShot, smaple);
				freeSample(smaple);
				Sleep(1000);
			}
			break;
		case 3:
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
			DLL_Dictionary_List = SnapshotTraversing(SnapShot_Head);
			calculateNumOfProcessInDll();
			Process_Dictionary_List = ProcessTraversing(SnapShot_Head);
			sort(tailSnapShot);
			CreateProjectPage();
			createsnapShotListxt();
			break;
		case 5:
			FreeMemoryAllocation(tailSnapShot);
			break;
		case 6:
			WriteToBinaryFile(tailSnapShot);
			break;
		case 7:
			LoadFromFile();
			break;
		case 8:
			exit(1);
			break;
		}
	} while (user_response != 8);


	return 0;
}