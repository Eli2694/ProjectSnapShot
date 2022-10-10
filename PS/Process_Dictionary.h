t_Processes_Dictionary* ProcessTraversing(t_SnapShot* SnapShotP);
void CreateListOfMonovalentProcessess(t_Process* SampleProcess);
int calculateNumOfMonovalentProcess();
void calculateAvgOfAvgWorkingSetSize(unsigned int sumOfProcesses);
extern unsigned long long AvgWorkingSetSize;
extern unsigned int sumOfProcesses;