#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "SnapShot.h"
#pragma warning(disable:4996)

//Function - Specification
void CreateListOfDlls(t_DLL* node);
t_Process* CreateListOfProcesses(t_Process* Process_node);


//Variable Declaration
t_DLL* DLL_Head = NULL;
t_DLL* DLL_Tail = NULL;
t_Process* Process_Head = NULL;
t_Process* Process_Tail = NULL;

t_Process* PrintMemoryInfo(DWORD processID)
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Open process in order to receive information
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (hProcess == NULL)
	{
		return NULL;
	}

	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR FoundProcessName[MAX_PATH];
	TCHAR FoundDllName[MAX_PATH];
	size_t numConverted;

	t_Process* process_node;
	t_DLL* Dll_node;

	DLL_Head = NULL;
	DLL_Tail = NULL;

	int i = 0; // Count Name Of DLLs
	int counterOfDlls = 0;

	process_node = (t_Process*)malloc(sizeof(t_Process));
	if (process_node == NULL)
	{
		LogError("Problam of memory allocation (process_node)",strerror(GetLastError()));
		return NULL;
	}

	// Get Process Name
	if (GetModuleFileNameEx(hProcess, 0, FoundProcessName, MAX_PATH))
	{
		wcstombs_s(&numConverted, process_node->ProcessName, MAX_PATH, FoundProcessName, MAX_PATH);

		if (!numConverted) // If The Process Without Name
		{
			free(process_node);
			return NULL;
		}
	}
	else
	{
		LogError(strerror(GetLastError()));
		return NULL;

	}
	// Get Process Data
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		process_node->ProcessId = processID;
		process_node->ProcessData = pmc;
	}
	else
	{
		LogError("Problam getting memory info of process",strerror(GetLastError()));
		return NULL;
	}

	// Get Dlls List
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			// Get the full path to the module's file.


			if (GetModuleFileNameEx(hProcess, hMods[i], FoundDllName, MAX_PATH))
			{

				//allocation of memory
				Dll_node = (t_DLL*)malloc(sizeof(t_DLL));
				if (Dll_node == NULL)
				{
					LogError("Problam of memory allocation (Dll_node)",strerror(GetLastError()));
					return NULL;
				}
				char* ret; // to use strstr and check if the DLL is standard
				// Convert wChar to regular char array (string)
				wcstombs_s(&numConverted, Dll_node->NameOfDLL, MAX_PATH, FoundDllName, MAX_PATH);
				if (strstr(Dll_node->NameOfDLL, ".dll") || strstr(Dll_node->NameOfDLL, ".DLL"))
				{
					counterOfDlls++;
					CreateListOfDlls(Dll_node);
				}
				else
				{
					free(Dll_node);

				}	
			}

		}
	}

	process_node->NumberOfDLLsInEachProcess = counterOfDlls;

	//Process Without DLLs
	if (i == 0)
	{
		process_node->ListOfDlls = NULL;
		process_node->NumberOfDLLsInEachProcess = 0;
	}

	//Inserting Start Of Dll List To Process Node
	process_node->ListOfDlls = DLL_Head;

	CloseHandle(hProcess);

	return process_node;
}

t_SnapShot* GetProcessesInfo()
{
	// Get Processes
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	t_SnapShot* SnapShot = NULL;
	t_Process* GetProcessNode;
	t_Process* GetHeadOfProcessList;
	//Every Time I Create New List Of Processes I Need To Reset The Beginning And End Of The List
	Process_Head = NULL;
	Process_Tail = NULL;
	int i; // i created this variable so that 'i' will not become zero(0) after i return from CreateListOfProcesses function to the loop - for (i = 0; i < cProcesses; i++)

	// Variable SnapShot Get Item Of Snapshot (Address of allocation of memory to t_SnapShot)
	

	// * Receive all process ID and put in aProcesses Array
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		LogError(strerror(GetLastError()));
		exit(1);
	}

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the memory usage for each process
	// *Loop of all processes
	for (i = 0; i < cProcesses; i++)
	{
		GetProcessNode = PrintMemoryInfo(aProcesses[i]);
		// Create List Of Processes
		if (GetProcessNode)
		{
			GetHeadOfProcessList = CreateListOfProcesses(GetProcessNode);
			//Create One SnapShot
			if (GetHeadOfProcessList)
			{
				SnapShot = OneSnapShot(GetHeadOfProcessList);
			}
		}
	}
	return SnapShot;
}

void CreateListOfDlls(t_DLL* Dll_node)
{
	if (DLL_Head == NULL)
	{
		Dll_node->next = NULL;
		Dll_node->prev = NULL;
		DLL_Head = Dll_node;
		DLL_Tail = Dll_node;
	}
	else
	{
		DLL_Tail->next = Dll_node;
		Dll_node->prev = DLL_Tail;
		Dll_node->next = NULL;
		DLL_Tail = Dll_node;
	}
}

t_Process* CreateListOfProcesses(t_Process* Process_node)
{
	t_Process* headAddress;

	if (Process_Head == NULL)
	{
		Process_node->next = NULL;
		Process_node->prev = NULL;
		Process_Tail = Process_node;
		Process_Head = Process_node;
		headAddress = Process_node;
	}
	else
	{
		Process_Tail->next = Process_node;
		Process_node->prev = Process_Tail;
		Process_node->next = NULL;
		Process_Tail = Process_node;
		return NULL;
	}
	return headAddress;
}

