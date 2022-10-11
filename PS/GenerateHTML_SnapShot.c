#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#pragma warning(disable:4996)

#define SEPERATOR "[seperator]"


int SaveIntoFile(char* fileName, char* buff)
{
	FILE* fi = fopen(fileName, "w");
	if (!fi)
	{
		return 0;
	}

	fputs(buff, fi);


	fclose(fi);
}
char* ReadAllFile(char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		return NULL;
	}


	// Get the file size
	char* buff = (char*)malloc(1000);
	char* read;
	int fileSize = 0;
	while ((read = fgets(buff, 1000, f)))
	{
		fileSize += strlen(buff);
	}

	free(buff);
	fclose(f);
	fileSize++;
	// alloc space as file size
	buff = (char*)malloc(fileSize);

	f = fopen(fileName, "r");
	if (!f)
	{
		return NULL;
	}
	int readPosition = 0;
	char charToRead;
	while ((charToRead = fgetc(f)) != EOF)
	{
		buff[readPosition] = charToRead;
		readPosition++;
	}
	buff[readPosition] = NULL;

	fclose(f);

	return buff;
}


void CreateProjectPage()
{
	//information to add - Application Statistics
	int sumOfDLLs = calculateSumOfDLLs();
	int sumOfProcesses = calculateNumOfMonovalentProcess();
	unsigned long long AvgWorkingSetSize = calculateAvgOfAvgWorkingSetSize(sumOfProcesses);
	char DLLsCount[10];
	char ProcessCount[10];
	char WorkingSet[30];
	sprintf(DLLsCount, "%d", sumOfDLLs);
	sprintf(ProcessCount, "%d", sumOfProcesses);
	sprintf(WorkingSet, "%d", AvgWorkingSetSize);


	// htmlTemplate = content of Project.html
	char* htmlTemplate = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\T_SnapShot.html");
	// find the token (---- [seperator] ---- )
	char* found = strstr(htmlTemplate, SEPERATOR);
	// Memory Allocation to fit the added information
	int len = found - htmlTemplate;
	char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(DLLsCount));
	//content of newFileSpace will be from the beginning of the page until the seperator - 1 part of the page template
	strncpy(newFileSpace, htmlTemplate, len);
	newFileSpace[len] = NULL;
	//Add new Content
	strcat(newFileSpace, DLLsCount);
	//add the the second part of the template
	strcat(newFileSpace, found + strlen(SEPERATOR));


	


	SaveIntoFile("SnapShot.html", newFileSpace);

	//free(newFileSpace);
	free(newFileSpace);
	free(htmlTemplate);

}