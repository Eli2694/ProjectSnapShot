t_SnapShot* OneSnapShot(t_Process* HeadOfProcessList);
t_SnapShot* ListOfSnapShots(t_SnapShot* OneSnapShot);
t_SnapShot* AggregationOfData(t_SnapShot* SnapShot_Tail, t_SnapShot* Sample);
void freeSample(t_SnapShot* sample);
int CountNumberOfProcess(t_Process* ListOfProcesses);
int CountNumberOfSnapShots(t_SnapShot* ListOfSnapShots);
t_SnapShot* SnapShot_Head;
t_SnapShot* SnapShot_Tail;