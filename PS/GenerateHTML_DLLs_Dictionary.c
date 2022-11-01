#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#include "DLL_Dictionary.h"
#include "Process_Dictionary.h"
#include "SnapShot.h"
#pragma warning(disable:4996)

//function - specifications
void inputListOfDlls();
int SaveIntoFile(char* fileName, char* buff);
char* ReadAllFile(char* fileName);
void CreateProjectPage();
void inputListOfSamples();
unsigned int AllNumOfDllsInSnapShot(t_SnapShot* curr);
unsigned long long AvgMemoryOfSnapShot(t_SnapShot* curr);

#define SEPERATOR "[seperator]"


int SaveIntoFile(char* fileName, char* htmlNewTemplate)
{
	FILE* fi = fopen(fileName, "w");
	if (!fi)
	{
		return 0;
	}

	fputs(htmlNewTemplate, fi);


	fclose(fi);
}
char* ReadAllFile(char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		LogError("Problem Of Opening A File(ReadAllFile function)");
		return NULL;
	}


	// Get the file size
	//fgets read rows (lines)
	char* buff = (char*)malloc(1000);
	if (buff == NULL)
	{
		LogError("Memory Allocation (GenerateHTML_DLLs_Dictionary)", strerror(GetLastError()));
		exit(1);
	}

	char* read;
	unsigned long fileSize = 0;
	while ((read = fgets(buff, 1000, f)))
	{
		fileSize += strlen(buff);
	}

	free(buff);
	fclose(f);
	fileSize++;
	// alloc space as file size
	buff = (char*)malloc(fileSize);
	if (buff == NULL)
	{
		LogError("Memory Allocation (GenerateHTML_DLLs_Dictionary)", strerror(GetLastError()));
		exit(1);
	}

	f = fopen(fileName, "r");
	if (!f)
	{
		LogError("Problem Of Opening A File(ReadAllFile function)");
		return NULL;
	}
	unsigned long readPosition = 0;
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
	char DLLsCount[6];
	char ProcessCount[6];
	char WorkingSet[15];
	sprintf(DLLsCount, "%d", sumOfDLLs);
	sprintf(ProcessCount, "%d", sumOfProcesses);
	sprintf(WorkingSet, "%lld", AvgWorkingSetSize);


	// htmlTemplate = content of Template Project.html
	char* htmlTemplate = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\T_Project.html");
	// find the token (---- [seperator] ---- )
	char* found = strstr(htmlTemplate, SEPERATOR);
	// found integer of charecters from the beginning of the html file until the seperator
	int len = found - htmlTemplate;
	// Memory Allocation to fit the added information
	char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(DLLsCount)); // Adding DLLs count
	if (newFileSpace == NULL) 
	{
		LogError("Problam of memory allocation - newFileSpace (CreateProjectPage)");
		exit(1);
		
	}
	//content of newFileSpace will be from the beginning of the page until the seperator - 1 part of the page template
	strncpy(newFileSpace, htmlTemplate, len);
	newFileSpace[len] = NULL;
	//Add new Content
	strcat(newFileSpace, DLLsCount);
	//add the the second part of the template
	strcat(newFileSpace, found + strlen(SEPERATOR)); // strlen(SEPERATOR) skip over [seperator] from html file
	
	//Adding monovalent Process count
	char* found2 = strstr(newFileSpace, SEPERATOR);
	int len2 = found2 - newFileSpace;
	char* newFileSpace2 = (char*)malloc(strlen(htmlTemplate) + strlen(ProcessCount));
	if (newFileSpace2 == NULL)
	{
		LogError("Problam of memory allocation - newFileSpace2 (CreateProjectPage)");
		exit(1);

	}
	strncpy(newFileSpace2, newFileSpace, len2);
	newFileSpace2[len2] = NULL;
	strcat(newFileSpace2, ProcessCount);
	strcat(newFileSpace2, found2 + strlen(SEPERATOR));
	
	//Adding Memory Avg
	char* found3 = strstr(newFileSpace2, SEPERATOR);
	int len3 = found3 - newFileSpace2;
	char* newFileSpace3 = (char*)malloc(strlen(htmlTemplate) + strlen(WorkingSet));
	if (newFileSpace3 == NULL)
	{
		LogError("Problam of memory allocation - newFileSpace3 (CreateProjectPage)");
		exit(1);

	}
	strncpy(newFileSpace3, newFileSpace2, len3);
	newFileSpace3[len3] = NULL;
	strcat(newFileSpace3, WorkingSet);
	strcat(newFileSpace3, found3 + strlen(SEPERATOR)); // newFileSpace3 - new template HTML

	//create SnapShot list
	inputListOfSamples();
	char* sample_list_info = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\txtFiles\\sample_list.txt");
	char* found4 = strstr(newFileSpace3, SEPERATOR);
	int len4 = found4 - newFileSpace3;
	char* newFileSpace4 = (char*)malloc(strlen(newFileSpace3) + strlen(sample_list_info));
	if (newFileSpace4 == NULL)
	{
		LogError("Problam of memory allocation - newFileSpace4 (CreateProjectPage)");
		exit(1);

	}
	strncpy(newFileSpace4, newFileSpace3, len4);
	newFileSpace4[len4] = NULL;
	strcat(newFileSpace4, sample_list_info);
	strcat(newFileSpace4, found4 + strlen(SEPERATOR));

	//create dll list
	// function that create file of type txt of - list of dlls - to read from
	inputListOfDlls();
	// dll_list_info will contain the information of dll_list txt file
	char* dll_list_info = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\txtFiles\\dll_list.txt");
	char* found5 = strstr(newFileSpace4, SEPERATOR);
	int len5 = found5 - newFileSpace4;
	char* newFileSpace5 = (char*)malloc(strlen(newFileSpace4) + strlen(dll_list_info));
	if (newFileSpace5 == NULL)
	{
		LogError("Problam of memory allocation - newFileSpace5 (CreateProjectPage)");
		exit(1);

	}
	strncpy(newFileSpace5, newFileSpace4, len5);
	newFileSpace5[len5] = NULL;
	strcat(newFileSpace5, dll_list_info);
	strcat(newFileSpace5, found5 + strlen(SEPERATOR));

	
	

	SaveIntoFile("C:\\Users\\User\\source\\repos\\PS\\PS\\HTML\\Project.html", newFileSpace5);

	//free(newFileSpace);
	free(newFileSpace);
	free(newFileSpace2);
	free(newFileSpace3);
	free(newFileSpace4);
	free(newFileSpace5);
	free(htmlTemplate);
	free(dll_list_info);
	free(sample_list_info);

}

void inputListOfDlls()
{
	FILE* out = fopen("C:\\Users\\User\\source\\repos\\PS\\PS\\txtFiles\\dll_list.txt", "w");
	if (!out)
	{
		return NULL;
	}
	int i = 1; // Process%d.html
	char DLLs[350];
	t_DLL_Dictionary* curr = DLL_DictionaryHead;
	while (curr)
	{
		//strcpy(name_of_dll, curr->Key_Dll_Name);
		sprintf(DLLs, "<tr><td class=\"name-of-dll\">%s</td> <td><a href=\"Process%d.html\">Processes that use this DLL </a></td></tr>", curr->Key_Dll_Name,i);
		fputs(DLLs, out);
		i++;
		curr = curr->next;
	}
	fclose(out);
}

void inputListOfSamples()
{
	FILE* out1 = fopen("C:\\Users\\User\\source\\repos\\PS\\PS\\txtFiles\\sample_list.txt", "w");
	if (!out1)
	{
		return NULL;
	}

	char numOfSamples[20];
	char linkToSample[60];
	char ProcessCount[22];
	char DllsCount[24];
	char MemoryAvgCount[30];
	unsigned int TotalDllCount;
	unsigned long long AvgMemory;

	t_SnapShot* temp = SnapShot_Head;
	t_SnapShot* curr = SnapShot_Head;

	while (curr)
	{
		
		sprintf(numOfSamples,"<td>%d</td>",curr->CountNumberOfSnapShot);
		sprintf(linkToSample, "<td><a href=\"SnapShot_%d.html\">SnapShot.%d</a> </td>", curr->CountNumberOfSnapShot, curr->CountNumberOfSnapShot);
		sprintf(ProcessCount, "<td>%d</td>", curr->CountNumberOfProcessesInEachSnapShot);
		TotalDllCount = AllNumOfDllsInSnapShot(curr);
		sprintf(DllsCount, "<td>%d</td>", TotalDllCount);
		AvgMemory = AvgMemoryOfSnapShot(curr);
		sprintf(MemoryAvgCount, "<td>%lld</td>", AvgMemory);

		fputs("<tr>", out1);
		fputs(numOfSamples, out1);
		fputs(linkToSample, out1);
		fputs(ProcessCount, out1);
		fputs(DllsCount, out1);
		fputs(MemoryAvgCount, out1);
		fputs("</tr>",out1);

		curr = curr->next;
	}

	fclose(out1);
}

unsigned int AllNumOfDllsInSnapShot(t_SnapShot* curr)
{
	unsigned int sumOfDllsInSnapShot = 0;
	t_SnapShot* sample = curr;
	t_Process* process;
	
	process = sample->ListOfProcesses;
	while (process)
	{
		sumOfDllsInSnapShot = sumOfDllsInSnapShot + process->NumberOfDLLsInEachProcess;
		process = process->next;
	}

	return sumOfDllsInSnapShot;
}

unsigned long long AvgMemoryOfSnapShot(t_SnapShot* curr)
{
	unsigned long long memoryAvg = 0;
	t_SnapShot* snapShot = curr;
	t_Process* processes;

	processes = snapShot->ListOfProcesses;
	while (processes)
	{
		memoryAvg = memoryAvg + processes->ProcessData.WorkingSetSize;
		processes = processes->next;
	}

	return memoryAvg / snapShot->CountNumberOfProcessesInEachSnapShot;
}