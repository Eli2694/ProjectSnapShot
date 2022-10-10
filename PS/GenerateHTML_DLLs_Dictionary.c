#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#include "DLL_Dictionary.h"
#include "Process_Dictionary.h"
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


void CreateNewHTMLtemplate()
{
	//information to add
	int sumOfDLLs = calculateSumOfDLLs();
	int sumOfProcesses = calculateNumOfMonovalentProcess();
	unsigned long long AvgWorkingSetSize = calculateAvgOfAvgWorkingSetSize(sumOfProcesses);
	char DLLsCount[10];
	char ProcessCount[10];
	char WorkingSet[20];
	sprintf(DLLsCount, "%d", sumOfDLLs);
	sprintf(ProcessCount, "%d", sumOfProcesses);
	sprintf(WorkingSet, "%d", AvgWorkingSetSize);


	// htmlTemplate = content of Project.html
	char* htmlTemplate = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\TProject.html");
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
	

	char* found2 = strstr(newFileSpace, SEPERATOR);
	int len2 = found2 - newFileSpace;
	char* newFileSpace2 = (char*)malloc(strlen(htmlTemplate) + strlen(ProcessCount));
	strncpy(newFileSpace2, newFileSpace, len2);
	newFileSpace2[len2] = NULL;
	strcat(newFileSpace2, ProcessCount);
	strcat(newFileSpace2, found2 + strlen(SEPERATOR));
	

	char* found3 = strstr(newFileSpace2, SEPERATOR);
	int len3 = found3 - newFileSpace2;
	char* newFileSpace3 = (char*)malloc(strlen(htmlTemplate) + strlen(WorkingSet));
	strncpy(newFileSpace3, newFileSpace2, len3);
	newFileSpace3[len3] = NULL;
	strcat(newFileSpace3, WorkingSet);
	strcat(newFileSpace3, found3 + strlen(SEPERATOR));
	

	SaveIntoFile("Project.html", newFileSpace3);

	//free(newFileSpace);
	free(newFileSpace3);
	free(htmlTemplate);


}