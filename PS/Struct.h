//The order of the structures is important because the software reads from top to bottom.
//For example : if the DLL structure was below the PROCESS structure, then the type DLL inside the PROCESS structure would have given an error.
#include <windows.h>
#include <psapi.h>

typedef struct s_DLL // dll description
{
	char NameOfDLL[MAX_PATH];
	struct s_DLL* next;
	struct s_DLL* prev;
}t_DLL;

typedef struct s_Process // process description
{
	unsigned long ProcessId;
	char ProcessName[MAX_PATH];
	PROCESS_MEMORY_COUNTERS ProcessData;
	unsigned int NumberOfDLLsInEachProcess;
	t_DLL* ListOfDlls;
	struct s_Process* next;
	struct s_Process* prev;
}t_Process;

typedef struct s_SnapShot // snapshot description
{
	t_Process* ListOfProcesses;
	unsigned int CountNumberOfSnapShot;
	unsigned int CountNumberOfProcessesInEachSnapShot;
	char TimeOfSnapShot[100];
	struct s_SnapShot* next;
	struct s_SnapShot* prev;
}t_SnapShot;

typedef struct s_HeaderFile
{
	int version;
	unsigned int SnapShotCount;
	char reservedSpace[20];
}t_HeaderFile;

typedef struct s_DLL_Dictionary
{
	char Key_Dll_Name[MAX_PATH];
	unsigned int NumOfProcess;
	t_Process* Process_List;
	struct s_DLL_Dictionary* next;
	struct s_DLL_Dictionary* prev;
}t_DLL_Dictionary;

typedef struct s_Processes_Dictionary
{
	char Key_Process_Name[MAX_PATH];
	SIZE_T WorkingSetSize;
	struct s_Processes_Dictionary* next;
	struct s_Processes_Dictionary* prev;
}t_Processes_Dictionary;