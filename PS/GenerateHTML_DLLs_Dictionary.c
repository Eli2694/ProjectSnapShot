#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#include "DLL_Dictionary.h"
#include "Process_Dictionary.h"
#pragma warning(disable:4996)

//function - specifications
void inputDictionaryDLLsList();
int SaveIntoFile(char* fileName, char* buff);
char* ReadAllFile(char* fileName);
void CreateProjectPage();

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
	char* htmlTemplate = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\T_Project.html");
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
	strcat(newFileSpace3, found3 + strlen(SEPERATOR)); // newFileSpace3 - new template HTML

	//create dll list
	inputDictionaryDLLsList();
	char* dll_list_info = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\dll_list.txt");
	char* found4 = strstr(newFileSpace3, SEPERATOR);
	int len4 = found4 - newFileSpace3;
	char* newFileSpace4 = (char*)malloc(strlen(newFileSpace3) + strlen(dll_list_info));
	strncpy(newFileSpace4, newFileSpace3, len4);
	newFileSpace4[len4] = NULL;
	strcat(newFileSpace4, dll_list_info);
	strcat(newFileSpace4, found4 + strlen(SEPERATOR));
	

	SaveIntoFile("Project.html", newFileSpace4);

	//free(newFileSpace);
	free(newFileSpace);
	free(newFileSpace2);
	free(newFileSpace3);
	free(htmlTemplate);

}

void inputDictionaryDLLsList()
{
	FILE* out = fopen("dll_list.txt", "w");
	if (!out)
	{
		return NULL;
	}

	char DLLs[500];
	char name_of_dll[100];
	char Processes[500];
	t_DLL_Dictionary* curr = DLL_DictionaryHead;
	t_Process* currProcesses;
	while (curr)
	{
		//strcpy(name_of_dll, curr->Key_Dll_Name);
		sprintf(DLLs, "<tr><td class=\"name-of-dll\">Name Of DLL</td><td class=\"name-of-dll\">%s</td></tr>", curr->Key_Dll_Name);
		fputs(DLLs, out);

		currProcesses = curr->Process_List;
		while (currProcesses)
		{
			sprintf(Processes, "<tr><td class=\"name-of-process\">Name Of Process</td><td class=\"name-of-process\">%s</td></tr>", currProcesses->ProcessName);
			fputs(Processes, out);

			currProcesses = currProcesses->next;
		}

		curr = curr->next;
	}
	fclose(out);
}