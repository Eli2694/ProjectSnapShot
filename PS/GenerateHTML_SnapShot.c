#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#include "SnapShot.h"
#include "GenerateHTML_DLLs_Dictionary.h"
#pragma warning(disable:4996)

#define _NO_CRT_STDIO_INLINE
#define SEPERATOR "[seperator]"

//function specification
void createSnapShotLisInHTML();
int FoundBiggestWorkingSetSize(t_Process* head);


int saveIntoFile(char* fileName, char* buff)
{
	FILE* fi = fopen(fileName, "w");
	if (!fi)
	{
		return 0;
	}

	fputs(buff, fi);


	fclose(fi);
}
char* readAllFile(char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		return NULL;
	}


	// Get the file size
	char* buff = (char*)malloc(1000);
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

	f = fopen(fileName, "r");
	if (!f)
	{
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



void createSnapShotLisInHTML()
{
	
	t_SnapShot* currSample = SnapShot_Head;
	t_Process* SampleProcess;
	t_DLL* DllSample;
	char process_name[300];
	char process_id[50];
	char pageFaultCount[50];
	char workingSetSize[150];
	char pageFileUsage[50];
	char quotaPagedPoolUsage[50];
	char quotaPeakPagePoolUsage[50];
	char dllCount[50];
	char td_dll[22] = "<td class=\"dll-list\">";
	char tdEnd[6] = "</td>";
	char select[30] = "<select class=\"select-list\">";
	char selectEnd[10] = "</select>";
	char option[300];
	char SampleListNumber[60];
	char* dataFromSmapleFile;
	char writeTxtFile[100];
	char readTxtFile[100];
	char HTMLfile[100];
	int fileCount = 0;
	int BiggestWorkingSetSizeLocation = 0;

	while (currSample)
	{
		

		fileCount++;

		sprintf(writeTxtFile, "C:\\Users\\User\\source\\repos\\PS\\PS\\txtFiles\\SnapShot_%d.txt", fileCount);
		//Openning File for writing into
		FILE* out = fopen(writeTxtFile, "w");
		if (!out)
		{
			return NULL;
		}

		// htmlTemplate hold address from heap so i need to free it at the end
		char* htmlTemplate = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\T_SnapShot.html");

		sprintf(SampleListNumber, "Sample List Number: %d AT %s", currSample->CountNumberOfSnapShot, currSample->TimeOfSnapShot);
		//search seperator
		char* found = strstr(htmlTemplate, SEPERATOR);
		// after subtraction of addresses , i will get number of characters from the beginning of the file to the separator
		int len = found - htmlTemplate;
		//allocation of memory for variable that will contain new template
		char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(SampleListNumber));
		if (newFileSpace == NULL)
		{
			LogError("Problam of memory allocation - newFileSpace (createSnapShotLisInHTML)");
			exit(1);

		}
		//input number of characters from the beginning of the file to the separator
		strncpy(newFileSpace, htmlTemplate, len);
		//strncpy does not input NULL at the end of string
		newFileSpace[len] = NULL;
		//adding string of dynamic SampleListNumber
		strcat(newFileSpace, SampleListNumber);
		//adding the second part of the template - after the seperator
		strcat(newFileSpace, found + strlen(SEPERATOR)); 
		//newFileSpace contain the template + SampleListNumber;
		//+ strlen(SEPERATOR) erase the SEPERATOR from html template 

		int icon = 0;

		SampleProcess = currSample->ListOfProcesses;
		BiggestWorkingSetSizeLocation = FoundBiggestWorkingSetSize(SampleProcess);

		while (SampleProcess)
		{

			icon++;
			
			if (BiggestWorkingSetSizeLocation == icon)
			{
				//Write to file to use the content for an HTML file
				fputs("<tr>", out);
				sprintf(process_name, "<td class=\"Process-Name\">%s</td>", SampleProcess->ProcessName);
				fputs(process_name, out);
				sprintf(process_id, "<td class=\"data\">%ld</td>", SampleProcess->ProcessId);
				fputs(process_id, out);
				sprintf(pageFaultCount, "<td class=\"data\">%ld</td>", SampleProcess->ProcessData.PageFaultCount);
				fputs(pageFaultCount, out);
				sprintf(workingSetSize, "<td class=\"data\">%lld<img src=\"../css/Images/warning-icon.png\"class=\"warning-icon\" /> </td>", SampleProcess->ProcessData.WorkingSetSize);
				fputs(workingSetSize, out);
				sprintf(pageFileUsage, "<td class=\"data\">%lld</tld>", SampleProcess->ProcessData.PagefileUsage);
				fputs(pageFileUsage, out);
				sprintf(quotaPagedPoolUsage, "<td class=\"data\">%lld</td>", SampleProcess->ProcessData.QuotaPagedPoolUsage);
				fputs(quotaPagedPoolUsage, out);
				sprintf(quotaPeakPagePoolUsage, "<td class=\"data\">%lld</td>", SampleProcess->ProcessData.QuotaPeakPagedPoolUsage);
				fputs(quotaPeakPagePoolUsage, out);
				sprintf(dllCount, "<td class=\"data\">%d</td>", SampleProcess->NumberOfDLLsInEachProcess);
				fputs(dllCount, out);

				// for list of dlls
				fputs(td_dll, out);
				fputs(select, out);
				fputs("<option class=\"list-option\">DLLs</option>", out);
			}
			else
			{
				//Write to file to use the content for an HTML file
				fputs("<tr>", out);
				sprintf(process_name, "<td class=\"Process-Name\">%s</td>", SampleProcess->ProcessName);
				fputs(process_name, out);
				sprintf(process_id, "<td class=\"data\">%ld</td>", SampleProcess->ProcessId);
				fputs(process_id, out);
				sprintf(pageFaultCount, "<td class=\"data\">%ld</td>", SampleProcess->ProcessData.PageFaultCount);
				fputs(pageFaultCount, out);
				sprintf(workingSetSize, "<td class=\"data\">%lld</td>", SampleProcess->ProcessData.WorkingSetSize);
				fputs(workingSetSize, out);
				sprintf(pageFileUsage, "<td class=\"data\">%lld</tld>", SampleProcess->ProcessData.PagefileUsage);
				fputs(pageFileUsage, out);
				sprintf(quotaPagedPoolUsage, "<td class=\"data\">%lld</td>", SampleProcess->ProcessData.QuotaPagedPoolUsage);
				fputs(quotaPagedPoolUsage, out);
				sprintf(quotaPeakPagePoolUsage, "<td class=\"data\">%lld</td>", SampleProcess->ProcessData.QuotaPeakPagedPoolUsage);
				fputs(quotaPeakPagePoolUsage, out);
				sprintf(dllCount, "<td class=\"data\">%d</td>", SampleProcess->NumberOfDLLsInEachProcess);
				fputs(dllCount, out);

				// for list of dlls
				fputs(td_dll, out);
				fputs(select, out);
				fputs("<option class=\"list-option\">DLLs</option>", out);
			}

			
			
			DllSample = SampleProcess->ListOfDlls;
			while (DllSample)
			{
				
				sprintf(option,"<option class=\"list-option\">%s</option>", DllSample->NameOfDLL);
				fputs(option, out);
				DllSample = DllSample->next;
			}

			fputs(selectEnd, out);
			fputs(tdEnd, out);
			fputs("</tr>", out);

			SampleProcess = SampleProcess->next; // next procees
			
			
			
		}

		//Closing file for writing
		fclose(out);

		//reading from file that contain list of processes and it's DLLs
		sprintf(readTxtFile, writeTxtFile);
		// dataFromSmapleFile contains list of process and its content - need to free
		dataFromSmapleFile = ReadAllFile(readTxtFile);
		char* found2 = strstr(newFileSpace, SEPERATOR);
		int len2 = found2 - newFileSpace;
		//newFileSpace2 - memory allocation - need to free 
		char* newFileSpace2 = (char*)malloc(strlen(newFileSpace) + strlen(dataFromSmapleFile) + 1);
		if (newFileSpace2 == NULL)
		{
			LogError("Problam of memory allocation - newFileSpace2 (createSnapShotLisInHTML)");
			exit(1);

		}
		//newFileSpace2 contain the details before the seperator
		strncpy(newFileSpace2, newFileSpace, len2);
		newFileSpace2[len2] = NULL;
		// input of data instead of the seperator
		strcat(newFileSpace2, dataFromSmapleFile);
		strcat(newFileSpace2, found2 + strlen(SEPERATOR)); // newFileSpace2 - current Template

		//Saving into HTML file
		sprintf(HTMLfile, "C:\\Users\\User\\source\\repos\\PS\\PS\\HTML\\SnapShot_%d.html", fileCount);
		SaveIntoFile(HTMLfile, newFileSpace2);

		free(newFileSpace2);
		free(dataFromSmapleFile);
		free(newFileSpace);
		free(htmlTemplate);
		
		currSample = currSample->next; // next sample
		
	}
}

int FoundBiggestWorkingSetSize(t_Process*head)
{
	int ret;
	int placementCount = 0;
	unsigned long long WorkingSetSize = 0;
	t_Process* curr = head;
	while (curr)
	{
		placementCount++;
		if (WorkingSetSize < curr->ProcessData.WorkingSetSize)
		{
			WorkingSetSize = curr->ProcessData.WorkingSetSize;
			ret = placementCount; // The purpose is to find where the process with the biggest WorkingSetSize
		}
		
		curr = curr->next;
	}
	return ret;
}