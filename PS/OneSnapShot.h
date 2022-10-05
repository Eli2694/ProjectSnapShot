t_SnapShot* OneSnapShot(t_Process* HeadOfProcessList);
t_SnapShot* ListOfSnapShots(t_SnapShot* OneSnapShot);
t_SnapShot* AggregationOfData(t_SnapShot* SnapShot_Tail, t_SnapShot* Sample);
void freeSample(t_SnapShot* sample);