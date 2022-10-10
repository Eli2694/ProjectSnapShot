#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#include "DLL_Dictionary.h"
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
	int sumOfDLLs = calculateSumOfDLLs();
	char DLLsCount[10];
	sprintf(DLLsCount, "%d", sumOfDLLs);

	char HTML_REPLACE[] = "<div>HAIM</div>";

	// htmlTemplate = content of Project.html
	char* htmlTemplate = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\Project.html");

	// find the token (---- [seperator] ---- )
	char* found = strstr(htmlTemplate, SEPERATOR);

	int len = found - htmlTemplate;
	char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(DLLsCount));

	strncpy(newFileSpace, htmlTemplate, len);
	newFileSpace[len] = NULL;

	strcat(newFileSpace, DLLsCount);
	

	strcat(newFileSpace, found + strlen(SEPERATOR));

	SaveIntoFile("newTest.html", newFileSpace);


	free(newFileSpace);
	free(htmlTemplate);

}