#include <vector>


#include "Job.h"
#include "JobQueueManager.h"
#include "BusyWorkManager.h"
//#include "Renderer.h"

//#include "SDL.h"

int main(int argc, char *argv[])
{
  JobQueueManager* pJobQueueManager = new JobQueueManager();
  BusyWorkManager* pBusyWorkManager = new BusyWorkManager(pJobQueueManager);
  //Renderer* pRenderer = new Renderer(pJobQueueManager);
  Job job;

  std::vector<Job> vThreadSpecificJobs;
  std::vector<Job> vDistributableJobs;
  job = Job(&BusyWorkManager::busyWork, pBusyWorkManager);
  vDistributableJobs.push_back(job);
  vDistributableJobs.push_back(job);
  vDistributableJobs.push_back(job);
  vDistributableJobs.push_back(job);

  //job = Job(&Renderer::run, pRenderer);
  //vThreadSpecificJobs.push_back(job);

  job = Job(&BusyWorkManager::killThreadDelay, pBusyWorkManager, "killThreadDelay");
  vThreadSpecificJobs.push_back(job);

  pJobQueueManager->startThreads(vThreadSpecificJobs, vDistributableJobs);
  
  


  delete pJobQueueManager;
  delete pBusyWorkManager;
  return 0;
}