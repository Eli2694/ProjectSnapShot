#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)

void Log(char str[])
{
	char Log[200];
	char file_Name[50];
	strcpy(Log, str);
	time_t t;
	time(&t);
	struct tm* timeInfo;
	timeInfo = localtime(&t);
	sprintf(Log, "%d/%d/%d - %02d:%02d:%02d - %s\n", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, str);
	sprintf(file_Name, "%02d-%02d-%d.log", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
	FILE* f = fopen(file_Name, "a");
	if (!f)
	{
		printf("Log - File Fail To Open");
		exit(1);
	}
	fputs(Log, f);
	fclose(f);
}

void LogError(char str[])
{
	char Error[100];
	sprintf(Error, "Error - %s", str);
	Log(Error);
}

void LogEvent(char str[])
{
	char Event[100];
	sprintf(Event, "Event - %s", str);
	Log(Event);
}