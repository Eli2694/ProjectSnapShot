#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "GetProcessInfo.h"
#include "SnapShot.h"
#include "SaveToFILE.h"
#include "AggregationOfData.h"
#include "resetCollection.h"
#pragma warning(disable:4996)

t_SnapShot* smaple;
t_SnapShot* tailSnapShot;

int main()
{
	int user_response = -1;
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
			break;
		case 4:
			break;
		case 5:
			FreeMemoryAllocation(tailSnapShot);
			break;
		case 6:
			WriteToBinaryFile(tailSnapShot);
			FreeMemoryAllocation(tailSnapShot);
			break;
		case 7:
			break;
		case 8:
			exit(1);
			break;
		}
	} while (user_response != 8);


	return 0;
}