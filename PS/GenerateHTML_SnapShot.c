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
void createsnapShotLisInHTML();


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



void createsnapShotLisInHTML()
{
	
	t_SnapShot* currSample = SnapShot_Head;
	t_Process* SampleProcess;
	t_DLL* DllSample;
	char process_name[500];
	char process_id[200];
	char pagefaultcount[200];
	char workingsetsize[200];
	char pagefileusage[200];
	char quotapagedpoolusage[200];
	char quotaPeakpagepoolusage[200];
	char dllcount[50];
	char td_dll[50] = "<td class=\"dll-list\">";
	char tdEnd[10] = "</td>";
	char select[50] = "<select class=\"select-list\">";
	char selectEnd[10] = "</select>";
	char option[300];
	char SampleListNumber[200];
	char* dataFromSmapleFile;
	char file_name[15] = "SnapShot_1.txt";
	char readFromfile[100];
	char HTMLfile[50];
	int k = 0;

	while (currSample)
	{
		

		k++;

		sprintf(file_name, "SnapShot_%d.txt",k);
		//Openning File for writing into
		FILE* out = fopen(file_name, "w");
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
		char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(SampleListNumber) +1 );
		//input number of characters from the beginning of the file to the separator
		strncpy(newFileSpace, htmlTemplate, len);
		//strncpy does not input NULL at the end of string
		newFileSpace[len] = NULL;
		//adding string of dynamic 
		strcat(newFileSpace, SampleListNumber);
		//adding the second part of the template - after the seperator
		strcat(newFileSpace, found + strlen(SEPERATOR)); 
		//newFileSpace contain the template + SampleListNumber;

		SampleProcess = currSample->ListOfProcesses;
		while (SampleProcess)
		{

			//condition to eliminate a problem when Number Of DLLs In Process is more then 10000
			if (SampleProcess->NumberOfDLLsInEachProcess > 10000)
			{
				break;
			}

			//Write to file to use the content for an HTML file
			fputs("<tr>", out);
			sprintf(process_name, "<td class=\"Process-Name\">%s</td>", SampleProcess->ProcessName);
			fputs(process_name, out);
			sprintf(process_id, "<td class=\"data\">%ld</td>", SampleProcess->ProcessId);
			fputs(process_id, out);
			sprintf(pagefaultcount, "<td class=\"data\">%ld</td>", SampleProcess->ProcessData.PageFaultCount);
			fputs(pagefaultcount, out);
			sprintf(workingsetsize, "<td class=\"data\">%lld</td>", SampleProcess->ProcessData.WorkingSetSize);
			fputs(workingsetsize, out);
			sprintf(pagefileusage, "<td class=\"data\">%lld</tld>", SampleProcess->ProcessData.PagefileUsage);
			fputs(pagefileusage, out);
			sprintf(quotapagedpoolusage, "<td class=\"data\">%lld</td>", SampleProcess->ProcessData.QuotaPagedPoolUsage);
			fputs(quotapagedpoolusage, out);
			sprintf(quotaPeakpagepoolusage, "<td class=\"data\">%lld</td>", SampleProcess->ProcessData.QuotaPeakPagedPoolUsage);
			fputs(quotaPeakpagepoolusage, out);
			sprintf(dllcount, "<td class=\"data\">%d</td>", SampleProcess->NumberOfDLLsInEachProcess);
			fputs(dllcount, out);

			// for list of dlls
			fputs(td_dll, out);
			fputs(select, out);

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
		sprintf(readFromfile,"C:\\Users\\User\\source\\repos\\PS\\PS\\%s", file_name);
		// dataFromSmapleFile contains list of process and its content - need to free
		dataFromSmapleFile = ReadAllFile(readFromfile);
		char* found2 = strstr(newFileSpace, SEPERATOR);
		int len2 = found2 - newFileSpace;
		//newFileSpace2 - memory allocation - need to free 
		char* newFileSpace2 = (char*)malloc(strlen(newFileSpace) + strlen(dataFromSmapleFile) + 1);
		//newFileSpace2 contain the details before the seperator
		strncpy(newFileSpace2, newFileSpace, len2);
		newFileSpace2[len2] = NULL;
		// input of data instead of the seperator
		strcat(newFileSpace2, dataFromSmapleFile);
		strcat(newFileSpace2, found2 + strlen(SEPERATOR)); // newFileSpace2 - current Template

		//Saving into HTML file
		sprintf(HTMLfile, "SnapShot_%d.html", k);
		SaveIntoFile(HTMLfile, newFileSpace2);

		free(newFileSpace2);
		free(dataFromSmapleFile);
		free(newFileSpace);
		free(htmlTemplate);
		
		currSample = currSample->next; // next sample
		
	}
}