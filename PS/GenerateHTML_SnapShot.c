#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Struct.h"
#include "Log.h"
#include "SnapShot.h"
#include "GenerateHTML_DLLs_Dictionary.h"
#pragma warning(disable:4996)

#define SEPERATOR "[seperator]"

//function specification
void createsnapShotListxt();


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



void createsnapShotListxt()
{
	
	t_SnapShot* currSample = SnapShot_Head;
	t_Process* SampleProcess;
	t_DLL* DllSample;
	char process_name[100];
	char process_id[50];
	char pagefaultcount[50];
	char workingsetsize[50];
	char pagefileusage[50];
	char quotapagedpoolusage[50];
	char quotaPeakpagepoolusage[50];
	char dllcount[50];
	char td_dll[50] = "<td class=\"dll-list\">";
	char tdEnd[10] = "</td>";
	char select[50] = "<select class=\"select-list\">";
	char selectEnd[10] = "</select>";
	char option[200];
	char SampleListNumber[100];
	char* dataFromSmapleFile;
	char file_name[15] = "SnapShot_1.txt";
	char readFromfile[50];
	char HTMLfile[15];
	int k = 1;

	while (currSample)
	{
		sprintf(file_name, "SnapShot_%d.txt",k);
		FILE* out = fopen(file_name, "w");
		if (!out)
		{
			return NULL;
		}


		char* htmlTemplate = ReadAllFile("C:\\Users\\User\\source\\repos\\PS\\PS\\T_SnapShot.html");
		sprintf(SampleListNumber, "Sample List Number: %d AT %s", currSample->CountNumberOfSnapShot, currSample->TimeOfSnapShot);
		char* found = strstr(htmlTemplate, SEPERATOR);
		int len = found - htmlTemplate;
		char* newFileSpace = (char*)malloc(strlen(htmlTemplate) + strlen(SampleListNumber));
		strncpy(newFileSpace, htmlTemplate, len);
		newFileSpace[len] = NULL;
		strcat(newFileSpace, SampleListNumber);
		strcat(newFileSpace, found + strlen(SEPERATOR)); // newFileSpace - current Template


		SampleProcess = currSample->ListOfProcesses;
		while (SampleProcess)
		{
			fputs("<tr>", out);
			sprintf(process_name, "<td class=\"Process-Name\">%s</td>", SampleProcess->ProcessName);
			fputs(process_name, out);
			sprintf(process_id, "<td class=\"data\">%d</td>", SampleProcess->ProcessId);
			fputs(process_id, out);
			sprintf(pagefaultcount, "<td class=\"data\">%d</td>", SampleProcess->ProcessData.PageFaultCount);
			fputs(pagefaultcount, out);
			sprintf(workingsetsize, "<td class=\"data\">%d</td>", SampleProcess->ProcessData.WorkingSetSize);
			fputs(workingsetsize, out);
			sprintf(pagefileusage, "<td class=\"data\">%d</td>", SampleProcess->ProcessData.PagefileUsage);
			fputs(pagefileusage, out);
			sprintf(quotapagedpoolusage, "<td class=\"data\">%d</td>", SampleProcess->ProcessData.QuotaPagedPoolUsage);
			fputs(quotapagedpoolusage, out);
			sprintf(quotaPeakpagepoolusage, "<td class=\"data\">%d</td>", SampleProcess->ProcessData.QuotaPeakPagedPoolUsage);
			fputs(quotaPeakpagepoolusage, out);
			sprintf(dllcount, "<td class=\"data\">%d</td>", SampleProcess->NumberOfDLLsInEachProcess);
			fputs(dllcount, out);

			// for list of dlls
			fputs(td_dll, out);
			fputs(select, out);

			DllSample = SampleProcess->ListOfDlls;
			while (DllSample)
			{
				sprintf(option, "<option class=\"list-option\">%s</option>", DllSample->NameOfDLL);

				DllSample = DllSample->next;
			}

			fputs(selectEnd, out);
			fputs(tdEnd, out);
			fputs("</tr>", out);

			SampleProcess = SampleProcess->next; // next procees
			
		}
		fclose(out);
		sprintf(readFromfile,"C:\\Users\\User\\source\\repos\\PS\\PS\\%s", file_name);
		dataFromSmapleFile = ReadAllFile(readFromfile);
		char* found2 = strstr(newFileSpace, SEPERATOR);
		int len2 = found2 - newFileSpace;
		char* newFileSpace2 = (char*)malloc(strlen(htmlTemplate) + strlen(dataFromSmapleFile));
		strncpy(newFileSpace2, newFileSpace, len2);
		newFileSpace2[len2] = NULL;
		strcat(newFileSpace2, dataFromSmapleFile);
		strcat(newFileSpace2, found2 + strlen(SEPERATOR)); // newFileSpace2 - current Template

		sprintf(HTMLfile, "SnapShot_%d.html", k);
		SaveIntoFile(HTMLfile, newFileSpace2);

		//free(newFileSpace);
		free(newFileSpace);
		free(newFileSpace2);
		
		

		currSample = currSample->next; // next sample
		k++;
	}
}