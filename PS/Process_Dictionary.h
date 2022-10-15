t_Processes_Dictionary* ProcessTraversing(t_SnapShot* SnapShotP);
void CreateListOfMonovalentProcessess(t_Process* SampleProcess);
int calculateNumOfMonovalentProcess();
unsigned long long calculateAvgOfAvgWorkingSetSize(unsigned int sumOfProcesses);
extern unsigned long long AvgWorkingSetSize;
extern unsigned int sumOfProcesses;
extern t_Processes_Dictionary* Process_DictionaryHead;
extern t_Processes_Dictionary* Process_DictionaryTail;