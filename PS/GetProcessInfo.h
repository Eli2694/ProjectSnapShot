t_SnapShot* GetProcessesInfo();
t_Process* CreateListOfProcesses(t_Process* Process_node);
void CreateListOfDlls(t_DLL* Dll_node);
t_DLL* DLL_Head;
t_DLL* DLL_Tail ;
extern t_Process* Process_Head;
extern t_Process* Process_Tail;
