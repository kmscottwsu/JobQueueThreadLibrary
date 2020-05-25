#include <vector>

#include "Job.h"
#include "JobQueueManager.h"
#include "BusyWorkManager.h"
//#include "Renderer.h"


int main()
{

  JobQueueManager* pJobQueueManager = new JobQueueManager();
  BusyWorkManager* pBusyWorkManager = new BusyWorkManager(pJobQueueManager);

  std::vector<Job> vThreadSpecificJobs;
  std::vector<Job> vDistributableJobs;
 Job job = Job(&BusyWorkManager::busyWork, pBusyWorkManager);



  //vThreadSpecificJobs.push_back(job);
  job = Job(&BusyWorkManager::killThreadDelay, pBusyWorkManager, "killThreadDelay");
  vThreadSpecificJobs.push_back(job);

  pJobQueueManager->startThreads(vThreadSpecificJobs, vDistributableJobs);
  
  


  delete pJobQueueManager;
  delete pBusyWorkManager;
  return 0;
}