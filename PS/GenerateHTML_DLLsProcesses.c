#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#include "SnapShot.h"
#include "DLL_Dictionary.h"
#include "GenerateHTML_DLLs_Dictionary.h"
#include "Process_Dictionary.h"
#pragma warning(disable:4996)

#define _NO_CRT_STDIO_INLINE
#define SEPERATOR "[seperator]"



//fucntion - specification
int countNumOfProcessesInDLL(t_Process* currDLL);


void createDLLsProcessListInHTML()
{

	typedef char ProcessData[50];

	char DllTitleInfo[300];
	char process_name[300];
	ProcessData process_id;
	ProcessData pageFaultCount;
	ProcessData workingSetSize;
	ProcessData pageFileUsage;
	ProcessData quotaPagedPoolUsage;
	ProcessData quotaPeakPagePoolUsage;
	
	int numOfFile = 0;
	int numProcesses;
	char* infoFromTxtFile;

	char writeTxtFile[100];
	char readTxtFile[100];
	char createHTMLFile[100];

	t_DLL_Dictionary* currDLL = DLL_DictionaryHead;
	t_Process* currProcess = currDLL->Process_List;

	while (currDLL)
	{

		numOfFile++;

		// htmlTemplate hold address from heap so i need to free it at the end
		char* htmlTemplate = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\T_ProcessesOfDLL.html");

		numProcesses = countNumOfProcessesInDLL(currDLL->Process_List);
		sprintf(DllTitleInfo, "%d Processes Used %s - DLL", currDLL->NumOfProcess, currDLL->Key_Dll_Name);

		//search seperator
		char* found = strstr(htmlTemplate, SEPERATOR);
		// after subtraction of addresses , i will get number of characters from the beginning of the file to the separator
		int len = found - htmlTemplate;
		//allocation of memory for variable that will contain new template
		char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(DllTitleInfo) + 1);
		if (newFileSpace == NULL)
		{
			LogError("Problam of memory allocation - newFileSpace (countNumOfProcessesInDLL)");
			exit(1);

		}
		//input number of characters from the beginning of the file to the separator
		strncpy(newFileSpace, htmlTemplate, len);
		//strncpy does not input NULL at the end of string
		newFileSpace[len] = NULL;
		//adding string of dynamic 
		strcat(newFileSpace, DllTitleInfo);
		strcat(newFileSpace, found + strlen(SEPERATOR));
		//newFileSpace contain the template + DllTitleInfo;

		sprintf(writeTxtFile,"C:\\Users\\User\\source\\repos\\PS\\PS\\txtFiles\\Process_%d.txt", numOfFile);
		//Openning File for writing into
		FILE* out = fopen(writeTxtFile, "w");
		if (!out)
		{
			return NULL;
		}

		currProcess = currDLL->Process_List;
		while (currProcess)
		{
			//Write to txt file to use the content for an HTML file
			fputs("<tr>", out);
			sprintf(process_name, "<td class=\"Process-Name\">%s</td>", currProcess->ProcessName);
			fputs(process_name, out);
			sprintf(process_id, "<td class=\"data\">%ld</td>", currProcess->ProcessId);
			fputs(process_id, out);
			sprintf(pageFaultCount, "<td class=\"data\">%ld</td>", currProcess->ProcessData.PageFaultCount);
			fputs(pageFaultCount, out);
			sprintf(workingSetSize, "<td class=\"data\">%lld</td>", currProcess->ProcessData.WorkingSetSize);
			fputs(workingSetSize, out);
			sprintf(pageFileUsage, "<td class=\"data\">%lld</tld>", currProcess->ProcessData.PagefileUsage);
			fputs(pageFileUsage, out);
			sprintf(quotaPagedPoolUsage, "<td class=\"data\">%lld</td>", currProcess->ProcessData.QuotaPagedPoolUsage);
			fputs(quotaPagedPoolUsage, out);
			sprintf(quotaPeakPagePoolUsage, "<td class=\"data\">%lld</td>", currProcess->ProcessData.QuotaPeakPagedPoolUsage);
			fputs(quotaPeakPagePoolUsage, out);
			fputs("</tr>", out);

			currProcess = currProcess->next;
			
		}

		//Closing file for writing
		fclose(out);
		//reading from file that contain list of processes and it's DLLs
		sprintf(readTxtFile,writeTxtFile);
		// dataFromSmapleFile contains list of process and its content - need to free
		infoFromTxtFile = ReadAllFile(readTxtFile);
		char* found2 = strstr(newFileSpace, SEPERATOR);
		int len2 = found2 - newFileSpace;
		//newFileSpace2 - memory allocation - need to free 
		char* newFileSpace2 = (char*)malloc(strlen(newFileSpace) + strlen(infoFromTxtFile));
		if (newFileSpace2 == NULL)
		{
			LogError("Problam of memory allocation - newFileSpace2 (countNumOfProcessesInDLL)");
			exit(1);

		}
		//newFileSpace2 contain the details before the seperator
		strncpy(newFileSpace2, newFileSpace, len2);
		newFileSpace2[len2] = NULL;
		// input of data instead of the seperator
		strcat(newFileSpace2, infoFromTxtFile);
		// newFileSpace2 - current Template
		strcat(newFileSpace2, found2 + strlen(SEPERATOR)); // strlen(SEPERATOR) SKIP OVER [seperator]: Does not record in the string inside newFileSpace2

		//Saving into HTML file
		sprintf(createHTMLFile, "C:\\Users\\User\\source\\repos\\PS\\PS\\HTML\\Process%d.html", numOfFile);
		SaveIntoFile(createHTMLFile, newFileSpace2);


		free(newFileSpace2);
		free(infoFromTxtFile);
		free(newFileSpace);
		free(htmlTemplate);


		currDLL = currDLL->next;
	}

	
}

int countNumOfProcessesInDLL(t_Process* currDLL)
{
	int numOfProcesses = 0;
	t_Process* curr = currDLL;
	while (curr)
	{
		numOfProcesses++;

		curr = curr->next;
	}

	return numOfProcesses;
}