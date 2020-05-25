#ifndef BUSY_WORK_MANAGER_H
#define BUSY_WORK_MANAGER_H

#include "StdLibraryDependencies.h"

#include "ManagerBaseClass.h"




class BusyWorkManager :
  public ManagerBaseClass
{
public:
  BusyWorkManager(JobQueueManager* pJobQueueManager);
  ~BusyWorkManager();

  void busyWork(JobOptions options);
  void killThreadDelay(JobOptions options);


private:
  std::mutex mut_;
  int i_;
};

#endif