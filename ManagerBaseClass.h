#ifndef MANAGER_BASE_CLASS_H
#define MANAGER_BASE_CLASS_H

#include "JobQueueManager.h"

class ManagerBaseClass
{
public:
  ManagerBaseClass()
  {
    pJobQueueManager_ = NULL;
  };

  ~ManagerBaseClass()
  {

  };

  JobQueueManager* pJobQueueManager_;
};

#endif